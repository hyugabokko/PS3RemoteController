
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

    const float ZERO_G = 511.5f; // 1.65/3.3*1023 (1.65V)

    //typedef ButtonEnum Button;

    class Controller {
        private:
            // structs
            struct StickXY { uint8_t x, y; };

        public:
	        // structs
            struct Stick { StickXY L, R; };
            struct Acc { int x, y, z; };
            struct Gyro { int z; };

            // var
            Stick stick;
            bool buttonPress[ buttonNum ];
            bool buttonClick[ buttonNum ];
            float pitch, roll;
            Acc acc;
            Gyro gyro;

            /////
            //  functions
            /////
            int init() { gyro.z = 0; return Usb.Init(); }
            void task() { Usb.Task(); }
            void disconnect() { PS3_Bt.disconnect(); }
            bool connected() { return PS3_Bt.PS3Connected; }
            //float getAngle( AngleEnum a ) { return PS3_Bt.getAngle( a ); };
            /* 
            センサに関して
            
            取得すると全体的にラグが発生するようになる。

            aX, aY, aZ
            100で1gっぽい。500で0g, 400で-1g
            軸の向きは、リモコンの手前側が+Y, 左が+X、上が+Z
            スタビライゼーションしているっぽい。値の更新が少し遅い

            gZ
            CWで+, CCWで-, 動いていない時は500
             */


            void update( bool rawSensorsVal = 0 )
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

                // get sensors
                if ( rawSensorsVal ) {
                    pitch = PS3_Bt.getAngle( Pitch );
                    roll = PS3_Bt.getAngle( Roll );
                    acc.x = PS3_Bt.getSensor( aX ) - ZERO_G;
                    acc.y = PS3_Bt.getSensor( aY ) - ZERO_G;
                    acc.z = PS3_Bt.getSensor( aZ ) - ZERO_G;
                    gyro.z = PS3_Bt.getSensor( gZ ) - ZERO_G;
                }
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
