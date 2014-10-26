#include "..\include\EndGameState.h"

//global variables for position calculating
extern const int screenWidth;
extern const int screenHeight;

EndGameState::EndGameState()
{
}


EndGameState::~EndGameState()
{
}

void EndGameState::Initialize()
{
	returnToMenu = false;
}

void EndGameState::Update(float a_timeStep, StateMachine* a_SMPointer)
{
	Input();
	if (returnToMenu)
	{
		a_SMPointer->PopState();
	}
}

void EndGameState::Draw()
{
	DrawString("GAME OVER", screenWidth * 0.5f - 100, screenHeight * .75f);

	DrawString("SCORE", screenWidth * .25, screenHeight * .50);
	char score[10];
	sprintf(score, "%05d", BaseState::score);
	DrawString(score, screenWidth *0.5 + 50, screenHeight * .50);

	DrawString("PRESS <ESC> TO RETURN TO MAIN MENU", screenWidth * .5 - 280, screenHeight * .1);
}

void EndGameState::Destroy()
{

}

//PRIVATE FUNCTIONS
void EndGameState::Input()
{
	if (IsKeyDown(GLFW_KEY_ESCAPE))
	{
		returnToMenu = true;
	}
}