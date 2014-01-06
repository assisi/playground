/*! \file  DiagnosticLed.h
    \brief A LED device used for diagnostic purposes
           (not visible by the bees)

    
*/

#ifndef ENKI_DIAGNOSTIC_LED_H
#define ENKI_DIAGNOSTIC_LED_H

#include <Interaction.h>

namespace Enki
{
    //! LED Diode useful for diagnostic purposes
    /*! Not visible by other objects

        TODO: This is a bit of a hack, because it changes
              the color of the whole object. Should be implemented
              nicer.
     */
    class DiagnosticLed : public LocalInteraction
    {
    public:
        DiagnosticLed(Robot* owner) : 
            LocalInteraction(0, owner),
            active_color_(owner->getColor()),
            inactive_color_(owner->getColor())
        {
            
        }

        void on(const Color& color)
        {
            owner->setColor(color);
            active_color_ = color;
        }

        void off(void)
        {
            owner->setColor(inactive_color_);
        }

                
    private:
            Color active_color_, inactive_color_;
    };
}

#endif

