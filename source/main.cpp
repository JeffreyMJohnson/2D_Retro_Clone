#include "AIE.h"
#include <iostream>
#include "StateMachine.h"
//for debug
#include "GameState.h"


using namespace std;

extern const int screenWidth = 1024;
extern const int screenHeight = 768;

//debug flag
bool debug = true;
int main( int argc, char* argv[] )
{	
    Initialise(screenWidth, screenHeight, false, "Galaxian Clone");
    SetBackgroundColour(SColour(0, 0, 0, 255));

	StateMachine state;
	if (debug)
	{
		state.PushState(new GameState());
	}
		
	

    //Game Loop
    do
	{
	   ClearScreen();
	   float delta;
	   if (debug)
	   {
		   //1/60 of a sec
		   delta = .01666666666;
	   }
	   else
	   {
		   delta = GetDeltaTime();
	   }

	   state.Update(delta);
	   state.Draw();


    } while(!FrameworkUpdate());

	
    Shutdown();

    return 0;
}
