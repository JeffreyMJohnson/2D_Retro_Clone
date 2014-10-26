#pragma once
#ifndef _MAIN_MENU_STATE_H_
#define _MAIN_MENU_STATE_H_

#include "BaseState.h"


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
};

#endif