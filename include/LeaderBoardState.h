#pragma once
#ifndef _LEADER_BOARD_STATE_H_
#define _LEADER_BOARD_STATE_H_

#include "BaseState.h"
#include "HighScores.h"
#include <list>

class LeaderBoardState :
	public BaseState
{
public:
	LeaderBoardState();
	~LeaderBoardState();

	void Initialize();
	void Update(float a_timeStep, StateMachine* a_SMPointer);
	void Draw();
	void Destroy();

private:
	void Input();
	unsigned int bannerSprite;

	std::list<int> scores;
	bool goBack;

};

#endif