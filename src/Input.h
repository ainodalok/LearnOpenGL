#ifndef INPUT_H
#define INPUT_H

#include <GLFW/glfw3.h>

#include <chrono>
#include <queue>
#include <unordered_map>
#include <iostream>

class Input
{
private:
	typedef int Key;

	typedef struct
	{
		int key;
		int action;
		std::chrono::high_resolution_clock::time_point timestamp;
	} KeyEvent;

	typedef struct
	{
		//Accumulates duration of several keypresses
		double duration; //After frame finishes becomes 0
		//Last press timestamp
		std::chrono::high_resolution_clock::time_point timestamp; //After frame finishes becomes frame end timestamp if still pressed
		bool pressed;
		bool pressedLastFrame; //After frame finishes becomes pressed
	} KeyData;

public:
	Input();
	
	void resolveKeyEvents();
	//Clean up duration, move timestamps of pressed keys to the beginning of the frame and assign pressedLastFrame the current value of pressed
	void frameFinished();
	//Returns duration in ms while the key was down during the frame, also adds to duration in case key is still pressed while duration is being queried
	double getKeyDuration(Key key);
	bool getKeySinglePressed(Key key);

	std::queue<KeyEvent> keyEvents;
	std::unordered_map<Key, KeyData> keyData;

private:
	void keyDown(int key, const std::chrono::high_resolution_clock::time_point& timestamp);
	void keyUp(int key, const std::chrono::high_resolution_clock::time_point& timestamp);
};
#endif