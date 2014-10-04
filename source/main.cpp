#include "AIE.h"
#include <iostream>


int maxWidth = 1024;
int maxHeight = 768;
int main( int argc, char* argv[] )
{	
    Initialise(maxWidth, maxHeight, false, "Test Game");
    
    SetBackgroundColour(SColour(0, 0, 0, 255));
float xPos = 100;
    float yPos = 100;
    int myTextureHandle = CreateSprite("./images/crate_sideup.png", xPos, yPos, false);
	int direction = 1;

    //Game Loop
    do
	{
		xPos += direction *1;
		if (xPos >= maxWidth || xPos< 0)
		{
			direction *= -1;
		}
		MoveSprite(myTextureHandle, xPos, yPos);
        DrawSprite(myTextureHandle);

        ClearScreen();

    } while(!FrameworkUpdate());

    Shutdown();

    return 0;
}
