#ifndef __ABSTRACT_GRID_PARALLEL_SIMULATION_H
#define __ABSTRACT_GRID_PARALLEL_SIMULATION_H

#include <boost/thread.hpp>
#include <boost/interprocess/sync/interprocess_semaphore.hpp>
#include <boost/foreach.hpp>

#include "interactions/AbstractGridSimulation.h"
#include "extensions/ExtendedWorld.h"

namespace Enki
{
	/**
	 * This class provides a thread manager to update grid cells in
	 * parallel.  The grid is divided in rectangular blocks.  Each block is
	 * assigned a worker thread.  The maximum number of worker threads is
	 * equal to the number of physical cores in the computer.  Every time
	 * step the main thread wakes up the worker threads so they update their
	 * respective block.  The main thread sleeps until all worker threads
	 * finish updating their blocks.
	 *
	 * <p> The worker thread semaphores and the main thread semaphore are
	 * initialised to zero.  In the beginning, the worker thread must wait
	 * for the signal from the main thread.  Also the main thread must wait
	 * for the worker threads to finish updating their rectangular blocks.
	 *
	 * <p> Template {@code class G} should be a specialisation of this class
	 * and should provide a method with the following signature: {@code void
	 * update(double deltaTime, int xmin, int ymin, int xmax, int ymax)}.
	 * This method receives the lower left and upper right corners of the
	 * rectangular block.
	 */
	template<class G, class T>
	class AbstractGridParallelSimulation :
		public AbstractGridSimulation<T>
	{
		/**
		 * Take into account processing of border grid cells.  If they should
		 * not be updated, adjust parameter {@code value}, otherwise just
		 * return this parameter.
		 */
		static int processBorder (int size, bool flag, int value)
		{
			if (!flag) {
				if (value == 0) {
					return 1;
				}
				else if (value == size) {
					return size - 1;
				}
			}
			return value;
		}
		/**
		 * Information used by each worker thread to update its rectangular
		 * block of the grid.  A worker thread sleeps until the main thread
		 * wakes it every time step.  The worker thread calls the update
		 * function and wakes up the main thread.
		 *
		 * <p> The update function is a function of class template {@code G}.
		 *
		 * <p> The rectangular block depends on grid size.  If the grid width
		 * is bigger than its height, the grid is divided in the horizontal
		 * axis, otherwise it is divided in the vertical axis.  Border grid
		 * cells may not be updated, so they can be excluded from the
		 * rectangular block.
		 */
		struct ThreadState
		{
			/**
			 * Left side of the  rectangular block.
			 */
			const int xmin;
			/**
			 * Right side of the rectangular block
			 */
			const int xmax;
			/**
			 * Lower side of the rectangular block.
			 */
			const int ymin;
			/**
			 * Upper side of the rectangular block.
			 */
			const int ymax;
			/**
			 * Delta time used by the function that updates grid cells.
			 */
			double deltaTime;
			/**
			 * The concrete class with the grid cell update function.
			 */
			G *grid;
			/**
			 * Worker thread sleep semaphore used to wait for the signal from
			 * the main thread.
			 */
			boost::interprocess::interprocess_semaphore wait;
			/**
			 * Main thread semaphore used to signal it that the worker thread
			 * has finished updating its rectangular block.
			 */
			boost::interprocess::interprocess_semaphore *fine;
			/**
			 * The thread that is using this thread information and state.
			 */
			boost::thread *thread;
			/**
			 * Construct a new worker thread information.
			 */
			ThreadState (G *ags, bool borderFlag, int index, int howMany, boost::interprocess::interprocess_semaphore *fine):
				xmin (processBorder (ags->size.x, borderFlag, ags->size.x > ags->size.y  ?   index      * ags->size.x / howMany  :  0)),
				xmax (processBorder (ags->size.x, borderFlag, ags->size.x > ags->size.y  ?  (index + 1) * ags->size.x / howMany  :  ags->size.x)),
				ymin (processBorder (ags->size.y, borderFlag, ags->size.x > ags->size.y  ?  0          :  index      * ags->size.y / howMany)),
				ymax (processBorder (ags->size.y, borderFlag, ags->size.x > ags->size.y  ?  ags->size.y  : (index + 1) * ags->size.y / howMany)),
				grid (ags),
				wait (0),
				fine (fine)
			{
			}
		};
		/**
		 * Array of worker thread information.
		 */
		std::vector<ThreadState *> threadsState;
		/**
		 * Semaphore used by the main thread to wait for every worker thread
		 * to finish updating their respective rectangular block of the grid.
		 */
		boost::interprocess::interprocess_semaphore *fine;
		/**
		 * Worker thread code.  The worker thread sleeps until the main
		 * thread wakes up to update its rectangular block.  After updating
		 * it wakes up the main thread.
		 */
		static void updatePartialGrid (ThreadState *threadState)
		{
			while (true) {
				threadState->wait.wait ();
				threadState->grid->updateGrid (threadState->deltaTime, threadState->xmin, threadState->ymin, threadState->xmax, threadState->ymax);
				threadState->fine->post ();
			}
		}
	protected:
		/**
		 * This constructor should be used by a class that inherit multiple
		 * times class {@code AbstractGrid}.
		 *
		 * @param parallelismLevel The parallelism level to be used:
		 * percentage of CPU threads to be used.
		 *
		 * @param grid The class with the grid cell update function
		 *
		 * @param borderFlag Whether border grid cells should be updated or not.
		 */
		AbstractGridParallelSimulation (double parallelismLevel, G *grid, bool borderFlag):
			AbstractGrid (NULL, -1, -1),
			AbstractGridSimulation<T> ()
		{
			this->initFields (parallelismLevel, grid, borderFlag);
		}
		/**
		 * Construct a new grid.
		 *
		 * @param parallelismLevel The parallelism level to be used:
		 * percentage of CPU threads to be used.
		 *
		 * @param grid The class with the grid cell update function
		 *
		 * @param borderFlag Whether border grid cells should be updated or not.
		 */
		AbstractGridParallelSimulation (const ExtendedWorld *world, double gridScale, double borderSize, double parallelismLevel, G *grid, bool borderFlag):
			AbstractGridSimulation<T> (world, gridScale, borderSize)
		{
			this->initFields (parallelismLevel, grid, borderFlag);
		}
		/**
		 * Destructor.
		 *
		 * TODO: stop threads.
		 */
		virtual ~AbstractGridParallelSimulation () {}

	public:
		/**
		 * Return the number of threads that are created for the given
		 * concurrency level.  A value of zero means no concurrency: there is
		 * only one thread.  A value of one means take advantage of all
		 * available CPU multi threading capabilities.
		 *
		 * @param parallelismLevel The parallelism level to be used.
		 */
		static unsigned int numberThreads (double parallelismLevel)
		{
			// unsigned int result = boost::thread::physical_concurrency ();
			unsigned int result = boost::thread::hardware_concurrency ();
			result = (unsigned int) (0.5 + result * parallelismLevel);
			return (result == 0 ? 1 : result);
		}
	private:
		/**
		 * Initialise instance fields after the constructor has calculated
		 * the grid size.
		 *
		 * <p> With C++11 this would be in the most general constructor.
		 */
		void initFields (double parallelismLevel, G *grid, bool borderFlag)
		{
			const unsigned int numberThreads = AbstractGridParallelSimulation::numberThreads (parallelismLevel);
			this->fine = new boost::interprocess::interprocess_semaphore (0);
			int i = numberThreads - 1;
			this->threadsState.reserve (numberThreads);
			while (i >= 0) {
				std::cout << "Created thread " << (numberThreads - i) << " of " << numberThreads << '\n';
				ThreadState *threadState = new ThreadState (grid, borderFlag, i, numberThreads, this->fine);
				threadState->thread = new boost::thread (AbstractGridParallelSimulation::updatePartialGrid, threadState);
				this->threadsState.push_back (threadState);
				i--;
			}
		}
	protected:
		/**
		 * Updates the grid cells.  Wake up all the worker threads and wait
		 * for them to finish updating their respective rectangular block.
		 * After that we update field {@code adtIndex}.
		 */
		void updateState (double deltaTime)
		{
			int nextAdtIndex = 1 - this->adtIndex;
			// wake up working threads
			BOOST_FOREACH (ThreadState *threadState, this->threadsState) {
				threadState->deltaTime = deltaTime;
				threadState->wait.post ();
			}
			// wait for working threads to finish update step
			for (int i = this->threadsState.size (); i > 0; i--) {
				this->fine->wait ();
			}
			this->adtIndex = nextAdtIndex;
		}
	};
}

#endif

// Local Variables:
// mode: c++
// mode: flyspell-prog
// ispell-local-dictionary: "british"
// End:
