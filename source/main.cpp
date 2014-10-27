#include "AIE.h"
#include <iostream>
#include "StateMachine.h"
#include "MainMenuState.h"

//for debug
#include "GameState.h"
#include "EndGameState.h"


using namespace std;

extern const int screenWidth = 600;
extern const int screenHeight = 800;
extern const int NUM_ENEMYS = 36;
extern const int NUM_ENEMY_COLS = 9;
extern const int NUM_ENEMY_ROWS = 4;

//extern const int NUM_ENEMYS = 18;


int main( int argc, char* argv[] )
{	
    Initialise(screenWidth, screenHeight, false, "Galaxian Clone");
    SetBackgroundColour(SColour(0, 0, 0, 255));

	StateMachine state;

	state.PushState(new MainMenuState());

    //Game Loop
    do
	{
	   ClearScreen();
	   float delta;
	   //DEBUG::REMOVE FOR RELEASE
	   //1/60 of a sec
		   delta = .01666666666;
		   //delta = GetDeltaTime();

	   state.Update(delta);
	   state.Draw();


    } while(!FrameworkUpdate() && !StateMachine::gameOver);

	
    Shutdown();

    return 0;
}
