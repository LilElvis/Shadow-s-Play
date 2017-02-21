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
	GameOver* gameOver = new GameOver;
	GameLevel* gameLevel = new GameLevel;
	
	//ADD STATES TO THE MANAGER
	Manager.AddGameState("MainMenu", mainMenu);
	Manager.AddGameState("GameOver", gameOver);
	Manager.AddGameState("GameLevel", gameLevel);

	//SET LEVELS BY DEFAULT TO PAUSED
	mainMenu->SetPaused(false);
	gameOver->SetPaused(true);
	gameLevel->SetPaused(true);

	//ACTIVATE WINDOW
	gameWindow->GetSFMLWindow()->setActive();
	gameWindow->GetSFMLWindow()->setFramerateLimit(60);

	//ENABLE GL FUNCTIONS
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//MAIN LOOP
	while (gameWindow->GetSFMLWindow()->isOpen())
	{
		Manager.Update();

		if (ENG::Input::GetKeyDown(ENG::KeyCode::Escape))
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