/* 
 * File:   ExtendedWorld.cpp
 * Author: pedro
 * 
 * Created on 17 de Fevereiro de 2014, 15:13
 */

#include "ExtendedWorld.h"

#include "interactions/VibrationSource.h"
#include "interactions/AirPump.h"
#include "interactions/NotSimulated.h"
#include "interactions/WorldHeat.h"

using namespace Enki;

ExtendedWorld::ExtendedWorld (double width, double height, 
                              const Color& wallsColor, 
                              const World::GroundTexture& groundTexture,
                              unsigned int skewMonitorRate,
                              double skewReportThreshold):
	World (width, height, wallsColor, groundTexture),
	SKEW_MONITOR_RATE (skewMonitorRate),
	SKEW_REPORT_THRESHOLD (skewReportThreshold + 1),
	worldHeat (NULL),
	absoluteTime (0)
{
}

ExtendedWorld::ExtendedWorld (double r, const Color& wallsColor,
                              const World::GroundTexture& groundTexture,
                              unsigned int skewMonitorRate,
                              double skewReportThreshold):
	World (r, wallsColor, groundTexture),
	SKEW_MONITOR_RATE (skewMonitorRate),
	SKEW_REPORT_THRESHOLD (skewReportThreshold + 1),
	worldHeat (NULL),
	absoluteTime (0)
{
}

ExtendedWorld::ExtendedWorld (
                              unsigned int skewMonitorRate,
                              double skewReportThreshold):
	World (),
	SKEW_MONITOR_RATE (skewMonitorRate),
	SKEW_REPORT_THRESHOLD (skewReportThreshold + 1),
	worldHeat (NULL),
	absoluteTime (0)
{
}

ExtendedWorld::~ExtendedWorld ()
{
}
void ExtendedWorld::addObject (PhysicalObject *o)
{
	World::addObject (o);
	ExtendedRobot *er = dynamic_cast<ExtendedRobot *> (o);
	if (er != NULL) {
		this->extendedRobots.insert (er);
	}
}

void ExtendedWorld::addPhysicSimulation (PhysicSimulation *pi)
{
	WorldHeat *newWorldHeat = dynamic_cast<WorldHeat *> (pi);
	if (newWorldHeat != NULL) {
		if (this->worldHeat != NULL) {
			// remove previous heat model
			PhysicSimulationsIterator iterator = this->physicSimulations.begin ();
			while (iterator != this->physicSimulations.end ()) {
				if (*iterator == this->worldHeat) {
					this->physicSimulations.erase (iterator);
					break;
				}
				iterator++;
			}
		}
		// update world heat model
		this->worldHeat = newWorldHeat;
	}
	this->physicSimulations.push_back (pi);
	pi->initParameters (this);
}

void ExtendedWorld::step (double dt, unsigned physicsOversampling)
{
	const double overSampledDt = dt / (double) physicsOversampling;
	for (unsigned po = 0; po < physicsOversampling; po++) {
		// init physics interactions
		for (PhysicSimulationsIterator pi = physicSimulations.begin (); pi != physicSimulations.end (); ++pi) {
			(*pi)->initStateComputing (overSampledDt);
			for (ExtendedRobotsIterator eri = extendedRobots.begin (); eri != extendedRobots.end (); ++eri) {
				(*eri)->initPhysicInteractions (overSampledDt, *pi);
				(*eri)->doPhysicInteractions (overSampledDt, *pi);
				(*eri)->finalizePhysicInteractions (overSampledDt, *pi);
			}
			(*pi)->computeNextState (overSampledDt);
		}
	}
	World::step (dt, physicsOversampling);
	absoluteTime += dt;
	// check skewness
	this->simulatedElapsedTime += dt;
	if (this->simulatedElapsedTime > this->SKEW_MONITOR_RATE) {
		boost::timer::cpu_times elapsed = this->skewTimer.elapsed ();
		double realElapsedTime = elapsed.wall / 1000000000.0;
		double skew = realElapsedTime / this->simulatedElapsedTime;
		if (skew > this->SKEW_REPORT_THRESHOLD) {
			std::cerr << "elapsed: " << elapsed.wall
			          << "\n simulated: " << simulatedElapsedTime
			          << "\n skew: " << skew << "  ~  " << this->SKEW_REPORT_THRESHOLD
			          << "\n";
			std::cerr << "Simulation time skew is ";
			std::cerr << std::fixed << std::setprecision (1) << (skew - 1) * 100;
			std::cerr << "%\n";
		}
		this->simulatedElapsedTime = 0;
		this->skewTimer.stop ();
		this->skewTimer.start ();
	}
}

double ExtendedWorld::getVibrationAmplitudeAt (const Point &position, double time) const
{
	double result = 0;
	for (ObjectsIterator i = this->objects.begin (); i != this->objects.end (); ++i) {
		PhysicalObject *po = (*i);
		VibrationSource *vibrationSource = dynamic_cast<VibrationSource *> (po);
		if (vibrationSource != NULL) {
			try {
				result += vibrationSource->getWaveAt (position, time);
			}
			catch (NotSimulated *ns) {
			}
		}
	}
	return result;
}

double ExtendedWorld::getAirFlowIntensityAt (const Point &position) const
{
	Vector result (0, 0);
	for (ObjectsIterator i = this->objects.begin (); i != this->objects.end (); ++i) {
		PhysicalObject *po = (*i);
		AirPump *airPump = dynamic_cast<AirPump *> (po);
		if (airPump != NULL) {
			try {
				result += airPump->getAirFlowAt (position);
			}
			catch (NotSimulated *ns) {
			}
		}
	}
	return result.norm ();
}
