// Invisible_Girl_Game.cpp : Defines the entry point for the application.
//

#include "Invisible_Girl_Game.h"
#include "Game.h"



using namespace std;


int main()
{
	Game game;

	while (game.IsRunning()) {
		game.CaptureInput();
		game.Update();
		game.LateUpdate();
		game.Draw();
		game.CalculateDeltaTime();
	}


	return 0;
}
