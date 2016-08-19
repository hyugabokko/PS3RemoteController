
#include "Arduino.h"

#include "PS3USB.h"
#include "PS3BT.h"

#include <usbhub.h>
#ifdef dobogusinclude
#include <spi4teensy3.h>
#include <SPI.h>
#endif

namespace PS3RemoteController
{
	USB Usb;
	PS3USB PS3_Usb( &Usb );
	BTD Btd( &Usb );
	PS3BT PS3_Bt( &Btd );

    typedef ButtonEnum Button;

    class Controller {
        private:

        public:
	        // structs
            struct Stick { uint8_t x, y; };
            struct Sticks { Stick L, R; };

            // var
            Sticks sticks;
            void update()
            {
    	        sticks.L.x = PS3_Bt.getAnalogHat( LeftHatX );
	            sticks.L.x = PS3_Bt.getAnalogHat( LeftHatY );
            }
    };
}

#define usb PS3RemoteController::Usb

#ifdef USBMODE
#define controller PS3RemoteController::PS3_Usb
#else
#define controller PS3RemoteController::PS3_Bt
#endif

//#define Button PS3RemoteController::Button

//#include "PS3RemoteController.cpp"
