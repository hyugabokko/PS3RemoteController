
#include "Arduino.h"

#ifdef PS3_USE_USB
#include "PS3USB.h"
#endif

#ifdef PS3_USE_BT
#include "PS3BT.h"
#endif

#include <usbhub.h>
#ifdef dobogusinclude
#include <spi4teensy3.h>
#include <SPI.h>
#endif

namespace PS3RemoteController
{
	USB Usb;

    #if defined( PS3_USE_USB )
	    PS3USB PS3_Usb( &Usb );
    #endif

    #if defined( PS3_USE_BT )
	    BTD Btd( &Usb );
	    PS3BT PS3_Bt( &Btd );
    #endif

    const unsigned short BUTTON_NUM = 17;

    const float ZERO_G = 511.5f; // 1.65/3.3*1023 (1.65V)

    //typedef ButtonEnum Button;

    class Controller {
        private:
            // structs
            struct StickXY { uint8_t x, y; };

            // enum
            enum MODE { PS3_BT, PS3_USB };

            bool calibrated = 0;

            MODE mode;

        public:
	        // structs
            struct Stick { StickXY L, R; };
            struct Acc { int x, y, z; };
            struct Gyro { int z; };

            // var
            Stick stick;
            bool buttonPress[ BUTTON_NUM ];
            bool buttonClick[ BUTTON_NUM ];
            float pitch, roll;
            Acc acc;
            Gyro gyro;
            Stick centerStick, maxStick, minStick;

            /////
            //  functions
            /////
            int init()
            {
                stick.L.x = 127;
                stick.L.y = 127;
                stick.R.x = 127;
                stick.R.y = 127;
                gyro.z = 0;
                return Usb.Init();
            }
            void task() { Usb.Task(); }
            void disconnect()
            {
                #if defined( PS3_USE_USB )
                    //PS3_Usb.disconnect();
                #elif defined( PS3_USE_BT )
                    PS3_Bt.disconnect();
                #elif defined( PS3_USE_USB ) && defined( PS3_USE_BT )
	                switch ( mode ) {
                        //case PS3_USB: PS3_Usb.disconnect(); break;
                        case PS3_BT: PS3_Bt.disconnect(); break;
                        default: break;
                    }
                #endif
            }
            bool connected()
            {
                #if defined( PS3_USE_USB )
                    if ( PS3_Usb.PS3Connected ) { mode = PS3_USB; return 1; }
                    else return 0;
                #elif defined( PS3_USE_BT )
                    if ( PS3_Bt.PS3Connected ) { mode = PS3_BT; return 1; }
                    else return 0;
                #elif defined( PS3_USE_USB ) && defined( PS3_USE_BT )
                    if ( PS3_Usb.PS3Connected ) { mode = PS3_USB; return 1; }
                    else if ( PS3_Bt.PS3Connected ) { mode = PS3_BT; return 1; }
                    else return 0;
                #endif
            }
            void setRumble( bool state )
            {
                #if defined( PS3_USB ) && defined( PS3_BT )
	                switch ( mode ) {
		                case PS3_USB: 
                            if ( state ) PS3_Usb.setRumbleOn( RumbleLow );
                            else PS3_Usb.setRumbleOff(); break;
                        case PS3_BT:
                            if ( state ) PS3_Bt.setRumbleOn( RumbleLow );
                            else PS3_Bt.setRumbleOff();  break;
                        default: break;
                    }
                #elif defined( PS3_USB )
                    if ( state ) PS3_Usb.setRumbleOn( RumbleLow );
                    else PS3_Usb.setRumbleOff();
                #elif defined( PS3_BT )
                    if ( state ) PS3_Bt.setRumbleOn( RumbleLow );
                    else PS3_Bt.setRumbleOff();
                #endif
            }
            void setRumbleMs( unsigned int ms )
            {
                setRumble( 1 ); delay( ms ); setRumble( 0 );
            } 
            void setLed( bool led1, bool led2, bool led3, bool led4 )
            {
                #if defined( PS3_USE_USB )
                    if ( led1 ) PS3_Usb.setLedOn( LED1 );
                    else PS3_Usb.setLedOff( LED1 );
                    if ( led2 ) PS3_Usb.setLedOn( LED2 );
                    else PS3_Usb.setLedOff( LED2 );
                    if ( led3 ) PS3_Usb.setLedOn( LED3 );
                    else PS3_Usb.setLedOff( LED3 );
                    if ( led4 ) PS3_Usb.setLedOn( LED4 );
                    else PS3_Usb.setLedOff( LED4 );
                #elif defined( PS3_USE_BT )
                    if ( led1 ) PS3_Bt.setLedOn( LED1 );
                    else PS3_Bt.setLedOff( LED1 );
                    if ( led2 ) PS3_Bt.setLedOn( LED2 );
                    else PS3_Bt.setLedOff( LED2 );
                    if ( led3 ) PS3_Bt.setLedOn( LED3 );
                    else PS3_Bt.setLedOff( LED3 );
                    if ( led4 ) PS3_Bt.setLedOn( LED4 );
                    else PS3_Bt.setLedOff( LED4 );
                #elif defined( PS3_USE_USB ) && defined( PS3_USE_BT )
	                switch ( mode ) {
		                case PS3_USB:
                            if ( led1 ) PS3_Usb.setLedOn( LED1 );
                            else PS3_Usb.setLedOff( LED1 );
                            if ( led2 ) PS3_Usb.setLedOn( LED2 );
                            else PS3_Usb.setLedOff( LED2 );
                            if ( led3 ) PS3_Usb.setLedOn( LED3 );
                            else PS3_Usb.setLedOff( LED3 );
                            if ( led4 ) PS3_Usb.setLedOn( LED4 );
                            else PS3_Usb.setLedOff( LED4 );
                            break;
                        case PS3_BT:
                            if ( led1 ) PS3_Bt.setLedOn( LED1 );
                            else PS3_Bt.setLedOff( LED1 );
                            if ( led2 ) PS3_Bt.setLedOn( LED2 );
                            else PS3_Bt.setLedOff( LED2 );
                            if ( led3 ) PS3_Bt.setLedOn( LED3 );
                            else PS3_Bt.setLedOff( LED3 );
                            if ( led4 ) PS3_Bt.setLedOn( LED4 );
                            else PS3_Bt.setLedOff( LED4 );
                            break;
                        default: break;
                    }
                #endif
            }
            void setLedToggle( bool led1, bool led2, bool led3, bool led4 )
            {
                #if defined( PS3_USE_USB )
                    if ( led1 ) PS3_Usb.setLedToggle( LED1 );
                    if ( led2 ) PS3_Usb.setLedToggle( LED2 );
                    if ( led3 ) PS3_Usb.setLedToggle( LED3 );
                    if ( led4 ) PS3_Usb.setLedToggle( LED4 );
                #elif defined( PS3_USE_BT )
                    if ( led1 ) PS3_Bt.setLedToggle( LED1 );
                    if ( led2 ) PS3_Bt.setLedToggle( LED2 );
                    if ( led3 ) PS3_Bt.setLedToggle( LED3 );
                    if ( led4 ) PS3_Bt.setLedToggle( LED4 );
                #elif defined( PS3_USE_USB ) && defined( PS3_USE_BT )
	                switch ( mode ) {
		                case PS3_USB:
                            if ( led1 ) PS3_Usb.setLedToggle( LED1 );
                            if ( led2 ) PS3_Usb.setLedToggle( LED2 );
                            if ( led3 ) PS3_Usb.setLedToggle( LED3 );
                            if ( led4 ) PS3_Usb.setLedToggle( LED4 ); break;
                        case PS3_BT:
                            if ( led1 ) PS3_Bt.setLedToggle( LED1 );
                            if ( led2 ) PS3_Bt.setLedToggle( LED2 );
                            if ( led3 ) PS3_Bt.setLedToggle( LED3 );
                            if ( led4 ) PS3_Bt.setLedToggle( LED4 ); break;
                        default: break;
                    }
                #endif
            }
            void update( bool sensorsVal = 0 )
            {
                // get button
                for ( int i = 0; i < BUTTON_NUM; i++ ) {
                    #if defined( PS3_USE_USB )
                        buttonPress[ i ] = PS3_Usb.getButtonPress( ( ButtonEnum )i );
                    #elif defined( PS3_USE_BT )
                        buttonPress[ i ] = PS3_Bt.getButtonPress( ( ButtonEnum )i );
                    #elif defined( PS3_USE_USB ) && defined( PS3_USE_BT )
	                    switch ( mode ) {
		                    case PS3_USB: buttonPress[ i ] = PS3_Usb.getButtonPress( ( ButtonEnum )i ); break;
                            case PS3_BT: buttonPress[ i ] = PS3_Bt.getButtonPress( ( ButtonEnum )i ); break;
                            default: break;
                        }
                    #endif
                }
                for ( int i = 0; i < BUTTON_NUM; i++ ) {
                    #if defined( PS3_USE_USB )
                        buttonClick[ i ] = PS3_Usb.getButtonClick( ( ButtonEnum )i );
                    #elif defined( PS3_USE_BT )
                        buttonClick[ i ] = PS3_Bt.getButtonClick( ( ButtonEnum )i );
                        Serial.println( "click update" );
                    #elif defined( PS3_USE_USB ) && defined( PS3_USE_BT )
	                    switch ( mode ) {
		                    case PS3_USB: buttonClick[ i ] = PS3_Usb.getButtonClick( ( ButtonEnum )i ); break;
                            case PS3_BT: buttonClick[ i ] = PS3_Bt.getButtonClick( ( ButtonEnum )i ); break;
                            default: break;
                        }
                    #endif
                }

                // get sticks
                #if defined( PS3_USE_USB )
                    stick.L.x = PS3_Usb.getAnalogHat( LeftHatX );
	                stick.L.y = PS3_Usb.getAnalogHat( LeftHatY );
    	            stick.R.x = PS3_Usb.getAnalogHat( RightHatX );
	                stick.R.y = PS3_Usb.getAnalogHat( RightHatY );
                #elif defined( PS3_USE_BT )
                    stick.L.x = PS3_Bt.getAnalogHat( LeftHatX );
	                stick.L.y = PS3_Bt.getAnalogHat( LeftHatY );
    	            stick.R.x = PS3_Bt.getAnalogHat( RightHatX );
	                stick.R.y = PS3_Bt.getAnalogHat( RightHatY );
                #elif defined( PS3_USE_USB ) && defined( PS3_USE_BT )
	                switch ( mode ) {
		                case PS3_USB:
                            stick.L.x = PS3_Usb.getAnalogHat( LeftHatX );
	                        stick.L.y = PS3_Usb.getAnalogHat( LeftHatY );
    	                    stick.R.x = PS3_Usb.getAnalogHat( RightHatX );
	                        stick.R.y = PS3_Usb.getAnalogHat( RightHatY ); break;
                        case PS3_BT: 
                            stick.L.x = PS3_Bt.getAnalogHat( LeftHatX );
	                        stick.L.y = PS3_Bt.getAnalogHat( LeftHatY );
    	                    stick.R.x = PS3_Bt.getAnalogHat( RightHatX );
	                        stick.R.y = PS3_Bt.getAnalogHat( RightHatY ); break;
                        default: break;
                    }
                #endif
    	        if ( calibrated ) {
                    stick.L.x = map( stick.L.x, 0, 255, minStick.L.x, maxStick.L.x );
                    stick.L.y = map( stick.L.y, 0, 255, minStick.L.y, maxStick.L.y );
                    stick.R.x = map( stick.R.x, 0, 255, minStick.R.x, maxStick.R.x );
                    stick.R.y = map( stick.R.y, 0, 255, minStick.R.y, maxStick.R.y );
                }
                
                // get sensors
                if ( sensorsVal ) {
                    #if defined( PS3_USE_USB )
                        pitch = PS3_Usb.getAngle( Pitch );
                        roll = PS3_Usb.getAngle( Roll );
                        acc.x = PS3_Usb.getSensor( aX ) - ZERO_G;
                        acc.y = PS3_Usb.getSensor( aY ) - ZERO_G;
                        acc.z = PS3_Usb.getSensor( aZ ) - ZERO_G;
                        gyro.z = PS3_Usb.getSensor( gZ ) - ZERO_G;
                    #elif defined( PS3_USE_BT )
                        pitch = PS3_Bt.getAngle( Pitch );
                        roll = PS3_Bt.getAngle( Roll );
                        acc.x = PS3_Bt.getSensor( aX ) - ZERO_G;
                        acc.y = PS3_Bt.getSensor( aY ) - ZERO_G;
                        acc.z = PS3_Bt.getSensor( aZ ) - ZERO_G;
                        gyro.z = PS3_Bt.getSensor( gZ ) - ZERO_G;
                    #elif defined( PS3_USE_USB ) && defined( PS3_USE_BT )
	                    switch ( mode ) {
		                    case PS3_USB:
                                pitch = PS3_Usb.getAngle( Pitch );
                                roll = PS3_Usb.getAngle( Roll );
                                acc.x = PS3_Usb.getSensor( aX ) - ZERO_G;
                                acc.y = PS3_Usb.getSensor( aY ) - ZERO_G;
                                acc.z = PS3_Usb.getSensor( aZ ) - ZERO_G;
                                gyro.z = PS3_Usb.getSensor( gZ ) - ZERO_G; break;
                            case PS3_BT: 
                                pitch = PS3_Bt.getAngle( Pitch );
                                roll = PS3_Bt.getAngle( Roll );
                                acc.x = PS3_Bt.getSensor( aX ) - ZERO_G;
                                acc.y = PS3_Bt.getSensor( aY ) - ZERO_G;
                                acc.z = PS3_Bt.getSensor( aZ ) - ZERO_G;
                                gyro.z = PS3_Bt.getSensor( gZ ) - ZERO_G; break;
                            default: break;
                        }
                    #endif
                }
            }
            void calibrate()
            {
                // start
                while ( buttonPress[ START ] && buttonPress[ SELECT ] ) { task(); update(); }
                setLed( 0, 0, 0 ,0 );
                maxStick.L.x = -1; maxStick.L.y = -1; maxStick.R.x = -1; maxStick.R.y = -1;
                minStick.L.x = -1; minStick.L.y = -1; minStick.R.x = -1; minStick.R.y = -1;
                setRumble( 1 );
                for ( int i = 0; i < 2; i++ ) {
                    task(); setLed( 1, 1, 1, 1 );// delay( 250 );
                    task(); setLed( 0, 0, 0, 0 );// delay( 250 );
                }
                setRumble( 0 );

                // center calibration
                setLed( 1, 0, 0, 0 );
                do { task(); update(); } while ( !buttonPress[ START ] || !buttonPress[ SELECT ] ) ;
                centerStick.L.x = stick.L.x; centerStick.L.y = stick.L.y;
                centerStick.R.x = stick.R.x; centerStick.R.y = stick.R.y;

                // max, min, and center calibration
                while ( buttonPress[ START ] && buttonPress[ SELECT ] ) { task(); update(); }
                setRumbleMs( 0 );
                setLed( 1, 1, 0, 0 );
                do {
                    task(); update();

                    if ( maxStick.L.x < stick.L.x ) maxStick.L.x = stick.L.x;
                    if ( maxStick.L.y < stick.L.y ) maxStick.L.y = stick.L.y;
                    if ( maxStick.R.x < stick.R.x ) maxStick.R.x = stick.R.x;
                    if ( maxStick.R.y < stick.R.y ) maxStick.R.y = stick.R.y;
                    if ( minStick.L.x > stick.L.x ) minStick.L.x = stick.L.x;
                    if ( minStick.L.y > stick.L.y ) minStick.L.y = stick.L.y;
                    if ( minStick.R.x > stick.R.x ) minStick.R.x = stick.R.x;
                    if ( minStick.R.y > stick.R.y ) minStick.R.y = stick.R.y;
                } while ( !buttonPress[ START ] || !buttonPress[ SELECT ] );
                // safety margin
                const unsigned short safetyMarginVal = 2;
                if ( maxStick.L.x <= 255 - safetyMarginVal ) maxStick.L.x += safetyMarginVal;
                if ( maxStick.L.y <= 255 - safetyMarginVal ) maxStick.L.y += safetyMarginVal;
                if ( maxStick.R.x <= 255 - safetyMarginVal ) maxStick.R.x += safetyMarginVal;
                if ( maxStick.R.y <= 255 - safetyMarginVal ) maxStick.R.y += safetyMarginVal;
                if ( minStick.L.x >= 0 + safetyMarginVal ) minStick.L.x -= safetyMarginVal;
                if ( minStick.L.y >= 0 + safetyMarginVal ) minStick.L.y -= safetyMarginVal;
                if ( minStick.R.x >= 0 + safetyMarginVal ) minStick.R.x -= safetyMarginVal;
                if ( minStick.R.y >= 0 + safetyMarginVal ) minStick.R.y -= safetyMarginVal;
                // average
                centerStick.L.x += stick.L.x; centerStick.L.y += stick.L.y;
                centerStick.R.x += stick.R.x; centerStick.R.y += stick.R.y;
                centerStick.L.x /= 2; centerStick.L.y /= 2;
                centerStick.R.x /= 2; centerStick.R.y /= 2;

                // end
                while ( buttonPress[ START ] && buttonPress[ SELECT ] ) { task(); update(); }
                setRumble( 1 );
                for ( int i = 0; i < 2; i++ ) {
                    task(); setLed( 1, 1, 1, 1 );// delay( 250 );
                    task(); setLed( 0, 0, 0, 0 );// delay( 250 );
                }
                setLed( 1, 0, 0, 0 );
                setRumble( 0 );
                calibrated = 1;
            }
    };
}
