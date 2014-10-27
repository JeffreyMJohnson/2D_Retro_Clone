#include "..\include\MainMenuState.h"

extern const int screenWidth;
extern const int screenHeight;

MainMenuState::MainMenuState()
{
	key = NONE;
}


MainMenuState::~MainMenuState()
{
}

void MainMenuState::Initialize()
{
	bannerSprite = CreateSprite("./images/misc/galaxian_banner.png", (float)screenWidth, screenHeight * .25f, true);
	MoveSprite(bannerSprite, screenWidth * .5f, screenHeight - (screenHeight * .25f) * .5f);
}
void MainMenuState::Update(float a_timeStep, StateMachine* a_SMPointer)
{
	Input();

	switch (key)
	{
	case NONE:
		break;
	case START:
		a_SMPointer->PushState(new GameState());
		break;
	case HIGH_SCORE:
		a_SMPointer->PushState(new LeaderBoardState());
		break;
	case QUIT:
		StateMachine::gameOver = true;
		a_SMPointer->PopState();
		break;
	}

}
void MainMenuState::Draw()
{
	DrawSprite(bannerSprite);
	DrawString("MAIN MENU", screenWidth * .5 - 80, screenHeight * .70f);
	DrawString("PRESS (S)TART NEW GAME", screenWidth * .5 - 200, screenHeight * .60);
	DrawString("PRESS (D)ISPLAY HIGH SCORES", screenWidth * .5 - 200, screenHeight * .60 - 50);
	DrawString("PRESS (Q)UIT GAME", screenWidth * .5 - 200, screenHeight * .60 - 100);
}
void MainMenuState::Destroy()
{

}

void  MainMenuState::Input()
{
	key = NONE;
	if (IsKeyDown(GLFW_KEY_S))
	{
		key = START;
	}

	if (IsKeyDown(GLFW_KEY_Q))
	{
		key = QUIT;
	}

	if (IsKeyDown(GLFW_KEY_D))
	{
		key = HIGH_SCORE;
	}
}