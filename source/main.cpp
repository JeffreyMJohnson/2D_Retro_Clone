#include "AIE.h"
#include <iostream>
#include "StateMachine.h"


using namespace std;

extern const int screenWidth = 1024;
extern const int screenHeight = 768;


int main( int argc, char* argv[] )
{	
    Initialise(maxWidth, maxHeight, false, "Test Game");
    SetBackgroundColour(SColour(0, 0, 0, 255));

	StateMachine state;

    //Game Loop
    do
	{
	   ClearScreen();



    } while(!FrameworkUpdate());

    Shutdown();

    return 0;
}
