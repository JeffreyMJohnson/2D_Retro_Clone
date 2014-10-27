#include "..\include\SplashScreenState.h"


SplashScreenState::SplashScreenState()
{
	splashTimer = 5.0f;
	currentSplashTime = 0.0f;
	dotTimer = 1.0f;
	currentDotTime = 0.0f;
	switchTime = false;
}


SplashScreenState::~SplashScreenState()
{

}

void SplashScreenState::Initialize()
{
	splashSpriteID = CreateSprite("./images/misc/galaxian_splash.png", screenWidth, screenHeight*.5f, true);
	MoveSprite(splashSpriteID, screenWidth * 0.5f, screenHeight * .5f);

	loadingMsg = "LOADING.";

}
void SplashScreenState::Update(float a_timeStep, StateMachine* a_SMPointer)
{
	if (switchTime)
	{
		a_SMPointer->SwitchState(new MainMenuState());
		return;
	}
	if (currentSplashTime < splashTimer)
	{
		currentSplashTime += a_timeStep;
		
		if (currentDotTime < dotTimer)
		{
			currentDotTime += a_timeStep;
		}
		else
		{
			loadingMsg += ".";
			currentDotTime = 0.0f;
		}
	}
	else
	{
		switchTime = true;
	}
}

void SplashScreenState::Draw()
{
	if (!switchTime)
	{
		DrawSprite(splashSpriteID);
		DrawString(loadingMsg.c_str(), screenWidth * 0.5f - 100, screenHeight *.25f - 20);
	}


}

void SplashScreenState::Destroy()
{

}