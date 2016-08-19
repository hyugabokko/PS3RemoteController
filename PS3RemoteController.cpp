
#include "PS3RemoteController.h"

PS3RemoteController::Controller::Controller()
{
	
}

int PS3RemoteController::Controller::init()
{
	return PS3RemoteController::Usb.Init();
}