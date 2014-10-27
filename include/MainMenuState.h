#pragma once
#ifndef _MAIN_MENU_STATE_H_
#define _MAIN_MENU_STATE_H_

#include "BaseState.h"
#include "GameState.h"
#include "LeaderBoardState.h"

enum PRESSED_KEY
{
	NONE,
	START, 
	HIGH_SCORE,
	QUIT
};

class MainMenuState :
	public BaseState
{
public:
	MainMenuState();
	~MainMenuState();

	void Initialize();
	void Update(float a_timeStep, StateMachine* a_SMPointer);
	void Draw();
	void Destroy();

private:
	void Input();

	unsigned int bannerSprite;

	PRESSED_KEY key;
};

#endif