
#pragma once

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

namespace PS3RemoteController
{
	static USB Usb;
#if USBMODE
	static PS3USB PS3( &Usb );
#else
	static BTD Btd( &Usb );
	static PS3BT PS3( &Btd );
#endif

	class Controller
	{
	private:


	public:

		Controller();
	};
}

//#include "PS3RemoteController.cpp"
