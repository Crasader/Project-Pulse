#ifndef _XBOX_CONTROLLER_H_
#define _XBOX_CONTROLLER_H_

#include "Windows.h"
#include "XInput.h"

#pragma comment(lib, "XInput.lib")

// XBOX Controller Class Definition
class XBOXController
{
public:
	XBOXController(int playerNumber);
	
	bool IsConnected();
	void updateState();
	void Vibrate(float leftVal = 0, float rightVal = 0);

	const int getControllerId() { return controllerId; }
	const XINPUT_STATE* getState() { return &controllerState; }
	const XINPUT_STATE* getLastState() { return &lastControllerState; }

private:
	XINPUT_STATE controllerState;
	XINPUT_STATE lastControllerState;
	int controllerId;
};

#endif