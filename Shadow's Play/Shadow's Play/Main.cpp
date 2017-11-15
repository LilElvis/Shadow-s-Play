//Cameron van Velzen - 100591663
#include "States.h"

int main()
{
	srand(time(NULL));

	//CREATE GAMESTATE MANAGER
	GameStateManager Manager;

	//INITIALIZE THE GAME
	Initialize();

	//CREATE STATE POINTERS
	//MainMenu* mainMenu = new MainMenu;
	//Tutorial* tutorial = new Tutorial;
	GameLevel* gameLevel = new GameLevel;
	//Credits* credits = new Credits;
	
	//ADD STATES TO THE MANAGER
	//Manager.AddGameState("MainMenu", mainMenu);
	//Manager.AddGameState("Tutorial", tutorial);
	Manager.AddGameState("GameLevel", gameLevel);
	//Manager.AddGameState("Credits", credits);

	//SET LEVELS BY DEFAULT TO PAUSED
	//mainMenu->SetPaused(true);
	//tutorial->SetPaused(true);
	gameLevel->SetPaused(false);
	//credits->SetPaused(true);

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