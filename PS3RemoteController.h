
#include "Arduino.h"

#if USBMODE
#include "PS3USB.h"
#else
#include "PS3BT.h"
#endif

#include <usbhub.h>
#ifdef dobogusinclude
#include <spi4teensy3.h>
#include <SPI.h>
#endif

class PS3RemoteController
{
private:


public:
	USB Usb;
#if USBMODE
	PS3USB PS3( &Usb );
#else
	BTD Btd( &Usb );
	PS3BT PS3( &Btd );
#endif


	PS3RemoteController();
};