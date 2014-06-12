#include "VibrationSource.h"

using namespace Enki;

VibrationSource::VibrationSource (double range, Robot* owner, Vector relativePosition, double orientation):
	LocalInteraction (range, owner),
	Component (owner, relativePosition, orientation)
{
	Component::init ();
	this->pos = Component::absolutePosition;
}

VibrationSource::VibrationSource (const VibrationSource &orig):
	LocalInteraction (orig.LocalInteraction::r, orig.LocalInteraction::owner),
	Component (orig)
{
}

VibrationSource::~VibrationSource ()
{
}

void VibrationSource::
init (double dt, Enki::World* w)
{
	Component::init ();
}

