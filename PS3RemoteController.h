
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

    const unsigned short buttonNum = 17;

    //typedef ButtonEnum Button;

    class Controller {
        private:
            // structs
            struct StickXY { uint8_t x, y; };

        public:
	        // structs
            struct Stick { StickXY L, R; };

            // var
            Stick stick;
            bool buttonPress[ buttonNum ];
            bool buttonClick[ buttonNum ];

            /////
            //  functions
            /////
            int init() { return Usb.Init(); }
            void task() { Usb.Task(); }
            void disconnect() { PS3_Bt.disconnect(); }
            bool connected() { return PS3_Bt.PS3Connected; }

            void update()
            {
                // get button
                for ( int i = 0; i < buttonNum; i++ )
                    buttonPress[ i ] = PS3_Bt.getButtonPress( ( ButtonEnum )i );
                for ( int i = 0; i < buttonNum; i++ )
                    buttonClick[ i ] = PS3_Bt.getButtonClick( ( ButtonEnum )i );

                // get sticks
    	        stick.L.x = PS3_Bt.getAnalogHat( LeftHatX );
	            stick.L.y = PS3_Bt.getAnalogHat( LeftHatY );
    	        stick.R.x = PS3_Bt.getAnalogHat( RightHatX );
	            stick.R.y = PS3_Bt.getAnalogHat( RightHatY );
            }
    };
}

//#define usb PS3RemoteController::Usb

//#ifdef USBMODE
//#define controller PS3RemoteController::PS3_Usb
//#else
//#define controller PS3RemoteController::PS3_Bt
//#endif

//#define Button PS3RemoteController::Button

//#include "PS3RemoteController.cpp"
