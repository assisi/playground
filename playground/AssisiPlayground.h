/*! \file   playground_ext.h
    \brief  A simple enki playground with an external interface.

 */

#ifndef ASSISI_PLAYGROUND_H
#define ASSISI_PLAYGROUND_H

// Enki includes
#include <viewer/Viewer.h>
#include <PhysicalEngine.h>
#include <robots/e-puck/EPuck.h>

// Qt includes
//#include <QApplication>
#include <QtGui>

#include <vector>

#include "interactions/WorldHeat.h"
#include "extensions/ExtendedWorld.h"

/*!	\file AssisiPlayground.h
	\brief The ASSISI Simulator.
*/

namespace Enki
{

	class AssisiPlayground : public ViewerWidget
	{
		
		Q_OBJECT;

		/**
		 * Data layers that can be drawn in the window.
		 */
		enum Layer {NONE, HEAT, ELECTRIC_FIELD, VIBRATION, LIGHT};
		/**
		 * Heat physic simulation used in the underlying world.
		 */
		const WorldHeat *worldHeat;
		/**
		 * Maximum presentable heat.  Ambient heat is drawn black, higher
		 * values are drawn in shades of red while lower values are drawn in
		 * shades of blue.
		 */
		const double maxHeat;
		const double maxVibration;
		/**
		 * The layer to drawn upon robots. 
		 */
		Layer layerToDraw;
		/**
		 * Transparency of the layer drawn above robots.
		 */
		double transparency;
		/**
		 * Whether to draw data using a gradient or not.
		 */
		bool useGradient;

		Point dataSize;
		std::vector<std::vector<std::vector<float> > > dataColour;
		/**
		 * Whether to show a help message or not.
		 */
		bool showHelp;
		/**
		 * The world that is shown in this widget.
		 */
		ExtendedWorld *extendedWorld;

	public:
		AssisiPlayground (ExtendedWorld *world, WorldHeat *worldHeat, QWidget *parent = 0);	
		void addEPuck (World *world, Point pos);

		~AssisiPlayground()
			{

			}
	
		// Inherited from viewer 
		virtual void sceneCompletedHook();
		virtual void renderObjectHook(PhysicalObject *object);

		void keyPressEvent (QKeyEvent *event);
	private:
		void drawVibrationLayer_Gradient ();
		void drawVibrationLayer_Chequerboard ();
		void drawHeatLayer_Chequerboard ();
		void drawHeatLayer_Gradient ();

		void setDataToHeat ();
		void setDataToVibration ();
		void drawDataAsGradient ();
		void drawDataAsCheckerBoard ();

	};

} // namespace Enki

#endif

// Local Variables: 
// mode: c++
// mode: flyspell-prog
// ispell-local-dictionary: "british"
// End: 
