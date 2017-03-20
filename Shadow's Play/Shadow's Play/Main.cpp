#include "States.h"

int main()
{
	srand(time(NULL));

	//CREATE GAMESTATE MANAGER
	GameStateManager Manager;

	//INITIALIZE THE GAME
	Initialize();

	//CREATE STATE POINTERS
	GameLevel* gameLevel = new GameLevel;
	
	//ADD STATES TO THE MANAGER
	Manager.AddGameState("GameLevel", gameLevel);

	//SET LEVELS BY DEFAULT TO PAUSED
	gameLevel->SetPaused(false);

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