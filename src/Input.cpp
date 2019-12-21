#include "Input.h"

Input::Input()
{
	keyData[GLFW_KEY_W] = { };
	keyData[GLFW_KEY_A] = { };
	keyData[GLFW_KEY_S] = { };
	keyData[GLFW_KEY_D] = { };
	keyData[GLFW_KEY_ESCAPE] = { };
}

void Input::resolveKeyEvents()
{
	while (!keyEvents.empty())
	{
		if (keyEvents.front().action == GLFW_PRESS)
			keyDown(keyEvents.front().key, keyEvents.front().timestamp);
		else if (keyEvents.front().action == GLFW_RELEASE)
			keyUp(keyEvents.front().key, keyEvents.front().timestamp);
		keyEvents.pop();
	}
}

void Input::keyDown(int key, const std::chrono::high_resolution_clock::time_point& timestamp)
{
	if (keyData.at(key).pressed)
		return;
	keyData.at(key).pressed = true;
	keyData.at(key).timestamp = timestamp;
}

void Input::keyUp(int key, const std::chrono::high_resolution_clock::time_point& timestamp)
{
	if (!keyData.at(key).pressed)
		return;
	keyData.at(key).pressed = false;
	keyData.at(key).duration += std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(timestamp - keyData.at(key).timestamp).count();
}

void Input::frameFinished()
{
	std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
	for (auto& [key, data] : keyData)
	{
		data.duration = 0;
		data.pressedLastFrame = data.pressed;
		if (data.pressed)
			data.timestamp = now;
	}
}

double Input::getKeyDuration(Key key)
{
	if (keyData.at(key).pressed)
	{
		std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
		keyData.at(key).duration += std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(now - keyData.at(key).timestamp).count();
		keyData.at(key).timestamp = now;
	}
	return keyData.at(key).duration;
}

bool Input::getKeySinglePressed(Key key)
{
	return (keyData.at(key).duration || keyData.at(key).pressed) && !keyData.at(key).pressedLastFrame;
}
