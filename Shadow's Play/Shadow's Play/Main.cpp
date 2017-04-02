#include "States.h"

int main()
{
	srand(time(NULL));

	//CREATE GAMESTATE MANAGER
	GameStateManager Manager;

	//INITIALIZE THE GAME
	Initialize();

	//CREATE STATE POINTERS
	MainMenu* mainMenu = new MainMenu;
	Tutorial* tutorial = new Tutorial;
	GameLevel* gameLevel = new GameLevel;
	
	//ADD STATES TO THE MANAGER
	Manager.AddGameState("MainMenu", mainMenu);
	Manager.AddGameState("Tutorial", tutorial);
	Manager.AddGameState("GameLevel", gameLevel);

	//SET LEVELS BY DEFAULT TO PAUSED
	mainMenu->SetPaused(false);
	tutorial->SetPaused(true);
	gameLevel->SetPaused(true);

	//ACTIVATE WINDOW
	gameWindow->GetSFMLWindow()->setActive();
	gameWindow->GetSFMLWindow()->setFramerateLimit(60);

	//ENABLE GL FUNCTIONS
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//MAIN LOOP
	while (gameWindow->GetSFMLWindow()->isOpen())
	{
		Manager.Update();
		Sound::systemUpdate();

		if (ENG::Input::GetKeyDown(ENG::KeyCode::Escape) || sf::Joystick::isButtonPressed(0, 6))
		{
			//CLEANUP HERE
			defaultMesh->Unload();
			defaultShader.unBind();
			defaultShader.unload();
			defaultTexture->Unload();
			return 0;
		}
	}
	//CLEANUP THERE
	defaultMesh->Unload();
	defaultShader.unBind();
	defaultShader.unload();
	defaultTexture->Unload();
	return 0;
}