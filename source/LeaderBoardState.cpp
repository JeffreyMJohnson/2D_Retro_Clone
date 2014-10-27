#include "..\include\LeaderBoardState.h"

extern const int screenWidth;
extern const int screenHeight;

LeaderBoardState::LeaderBoardState()
{
}


LeaderBoardState::~LeaderBoardState()
{
}

void LeaderBoardState::Initialize()
{
	bannerSprite = CreateSprite("./images/misc/galaxian_banner.png", (float)screenWidth, screenHeight * .25f, true);
	MoveSprite(bannerSprite, screenWidth * .5f, screenHeight - (screenHeight * .25f) * .5f);

	HighScores hScores;
	hScores.LoadScores();
	scores = hScores.GetScores();
	goBack = false;
}
void LeaderBoardState::Update(float a_timeStep, StateMachine* a_SMPointer)
{
	Input();
	if (goBack)
	{
		a_SMPointer->PopState();
	}
}
void LeaderBoardState::Draw()
{
	DrawSprite(bannerSprite);
	DrawString("HIGH SCORES", screenWidth * .5f - 100, screenHeight * .70f);
	float yPos = screenHeight * .65f;
	float xPos = screenWidth * .5f - 50;

	for (std::list<int>::iterator it = scores.begin(); it != scores.end(); it++)
	{
		char s[10];
		sprintf(s, "%5d", *it);
		DrawString(s, xPos, yPos);
		yPos -= 35;
	}

	DrawString("PRESS <ESC> TO RETURN TO MAIN MENU", screenWidth * .5 - 280, screenHeight * .1);
}
void LeaderBoardState::Destroy()
{

}

void  LeaderBoardState::Input()
{
	if (IsKeyDown(GLFW_KEY_ESCAPE))
	{
		goBack = true;
	}
}