#include "CXBOXController.h"

XBOXController::XBOXController(int playerNumber)
{
	controllerId = playerNumber - 1;
}

bool XBOXController::IsConnected()
{
	ZeroMemory(&controllerState, sizeof(XINPUT_STATE));

	DWORD result = XInputGetState(controllerId, &controllerState);

	return result == ERROR_SUCCESS;
}

void XBOXController::updateState()
{
	lastControllerState = controllerState;

	ZeroMemory(&controllerState, sizeof(XINPUT_STATE));

	XInputGetState(controllerId, &controllerState);
}

void XBOXController::Vibrate(float leftVal, float rightVal)
{
	leftVal = leftVal < 0 ? 0 : leftVal > 1 ? 1 : leftVal;
	rightVal = rightVal < 0 ? 0 : rightVal > 1 ? 1 : rightVal;

	XINPUT_VIBRATION vibration;

	ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));

	vibration.wLeftMotorSpeed = int(leftVal * leftVal * (unsigned short) -1);
	vibration.wRightMotorSpeed = int(rightVal * rightVal * (unsigned short) -1);

	XInputSetState(controllerId, &vibration);
}