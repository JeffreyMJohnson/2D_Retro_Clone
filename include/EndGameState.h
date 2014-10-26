#pragma once

#ifndef _END_GAME_STATE_H_
#define _END_GAME_STATE_H_

#include "BaseState.h"
#include <string>

class EndGameState :
	public BaseState
{
public:
	EndGameState();
	~EndGameState();

	void Initialize();
	void Update(float a_timeStep, StateMachine* a_SMPointer);
	void Draw();
	void Destroy();

private:
	void Input();
	bool returnToMenu;



};



#endif
