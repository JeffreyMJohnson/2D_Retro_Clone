#pragma once
#ifndef _SPLASH_SCREEN_STATE_H_
#define _SPLASH_SCREEN_STATE_H_

#include "BaseState.h"
#include "Utility.h"
#include "MainMenuState.h"
#include <string>

//global variables for position calculating
extern const int screenWidth;
extern const int screenHeight;

class SplashScreenState :
	public BaseState
{
public:
	SplashScreenState();
	~SplashScreenState();

	void Initialize();
	void Update(float a_timeStep, StateMachine* a_SMPointer);
	void Draw();
	void Destroy();

private:
	float splashTimer;
	float currentSplashTime;
	float dotTimer;
	float currentDotTime;
	unsigned int splashSpriteID;
	std::string loadingMsg;
	bool switchTime;
	
};

#endif