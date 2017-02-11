#include "States.h"

#define LIT true
#define litMotherfucker isLit

void Initialize()
{
	//SETUP
	Settings.depthBits = 24;
	Settings.stencilBits = 8;
	Settings.antialiasingLevel = 2;

	//CREATE WINDOW
	gameWindow->GetSFMLWindow()->create(
		sf::VideoMode(windowWidth, windowHeight), "Shadow's Play", sf::Style::Close, Settings);

	//CHECK IF GLEW INITIALIZED SUCCESSFULLY
	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW could not be initialized.\n";
		system("PAUSE");
		exit(0);
	}

	//SET CAMERA
	cameraPos = glm::vec3(0.0f, 0.0f, 75.0f);

	cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	cameraDirection = glm::normalize(cameraPos - cameraTarget);

	up = glm::vec3(0.0f, 1.0f, 0.0f);
	down = glm::vec4(0.0f, -1.0f, 0.0f, 1.0f);
	cameraRight = glm::normalize(glm::cross(up, cameraDirection));

	cameraUp = glm::cross(cameraDirection, cameraRight);

	cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	
	view.setMatrix(glm::lookAt(cameraPos, (cameraPos + cameraFront), up));
	view.rotateX(-1.57f);
	
	persp = glm::perspective(45.0f, 16.0f / 9.0f, 0.001f, 200.0f);
	
	//LOAD MENU TEXTURES
	defaultMesh->LoadFromFile("Quad", "../assets/objects/Quad.obj");
	defaultTexture->LoadFromFile("MainMenu", "../assets/textures/Menu.png");
	defaultTexture->LoadFromFile("GameOver", "../assets/textures/GameOver.png");
	static ENG::SceneObject Quad("Quad", defaultMesh->listOfMeshes["Quad"]->VAO, defaultTexture->listOfTextures["MainMenu"]->getSFTexture());
	sceneObjects["Quad"] = &Quad;
	static ENG::SceneObject Quad2("Quad", defaultMesh->listOfMeshes["Quad"]->VAO, defaultTexture->listOfTextures["GameOver"]->getSFTexture());
	sceneObjects["Quad2"] = &Quad2;

	//LOAD OBJECTS
	defaultMesh->LoadFromFile("Nyx", "../assets/objects/Nyx/Nyx.obj");
	defaultTexture->LoadFromFile("Nyx", "../assets/textures/Nyx.png");
	static ENG::Player Nyx("Nyx", defaultMesh->listOfMeshes["Nyx"]->VAO, defaultTexture->listOfTextures["Nyx"]->getSFTexture());
	Player["Nyx"] = &Nyx;

	defaultMesh->LoadFromFile("Room", "../assets/objects/MapTemp.obj");
	defaultTexture->LoadFromFile("Room", "../assets/textures/MapTempTest.png");
	static ENG::SceneObject Room("Room", defaultMesh->listOfMeshes["Room"]->VAO, defaultTexture->listOfTextures["Room"]->getSFTexture());
	sceneObjects["Room"] = &Room;

	defaultMesh->LoadFromFile("Warning", "../assets/objects/SpotLight.obj");
	defaultTexture->LoadFromFile("Warning", "../assets/textures/Warning.png");
	static ENG::SceneObject Warning("Warning", defaultMesh->listOfMeshes["Warning"]->VAO, defaultTexture->listOfTextures["Warning"]->getSFTexture());
	sceneObjects["Warning"] = &Warning;
	static ENG::SceneObject Warning2("Warning", defaultMesh->listOfMeshes["Warning"]->VAO, defaultTexture->listOfTextures["Warning"]->getSFTexture());
	sceneObjects["Warning2"] = &Warning2;
	

	defaultMesh->LoadFromFile("SpotLight", "../assets/objects/SpotLight.obj");
	defaultTexture->LoadFromFile("SpotLight", "../assets/textures/Light1.png");
	defaultTexture->LoadFromFile("SpotLight2", "../assets/textures/Light2.png");
	static ENG::SceneObject SpotLight("SpotLight", defaultMesh->listOfMeshes["SpotLight"]->VAO, defaultTexture->listOfTextures["SpotLight"]->getSFTexture());
	sceneObjects["SpotLight"] = &SpotLight;
	static ENG::SceneObject SpotLight2("SpotLight", defaultMesh->listOfMeshes["SpotLight"]->VAO, defaultTexture->listOfTextures["SpotLight2"]->getSFTexture());
	sceneObjects["SpotLight2"] = &SpotLight2;

	defaultMesh->LoadFromFile("QuadLight", "../assets/objects/QuadLight.obj");
	defaultTexture->LoadFromFile("QuadLight", "../assets/textures/QuadLight.png");
	static ENG::SceneObject QuadLight("QuadLight", defaultMesh->listOfMeshes["QuadLight"]->VAO, defaultTexture->listOfTextures["QuadLight"]->getSFTexture());
	sceneObjects["QuadLight"] = &QuadLight;
	defaultTexture->LoadFromFile("QuadWarning", "../assets/textures/QuadWarning.png");
	static ENG::SceneObject Warning3("Warning", defaultMesh->listOfMeshes["QuadLight"]->VAO, defaultTexture->listOfTextures["QuadWarning"]->getSFTexture());
	sceneObjects["Warning3"] = &Warning3;

	static ENG::SceneObject InvisWall("InvisWall", 0, defaultTexture->listOfTextures["SpotLight"]->getSFTexture());
	sceneObjects["InvisWall"] = &InvisWall;
	
	static ENG::SceneObject InvisWall2("InvisWall2", 0, defaultTexture->listOfTextures["SpotLight"]->getSFTexture());
	sceneObjects["InvisWall2"] = &InvisWall2;
	
	static ENG::SceneObject InvisWall3("InvisWall3", 0, defaultTexture->listOfTextures["SpotLight"]->getSFTexture());
	sceneObjects["InvisWall3"] = &InvisWall3;
	
	static ENG::SceneObject InvisWall4("InvisWall4", 0, defaultTexture->listOfTextures["SpotLight"]->getSFTexture());
	sceneObjects["InvisWall4"] = &InvisWall4;

	//LOAD SHADER PROGRAMS
	defaultShader.load("Default", "../assets/shaders/StaticGeometry.vert", "../assets/shaders/BasicLighting.frag");
	passThrough.load("PassThrough", "../assets/shaders/PassThrough.vert", "../assets/shaders/PassThrough.frag");

	//INITIALIZE/LOAD SOUNDS
	soundEngine.Init();
	Sounds["bgm"] = new Sound();
	Sounds["bgm"]->load("../assets/sounds/bgm.wav", true);
	
	Sounds["die"] = new Sound();
	Sounds["die"]->load("../assets/sounds/die.wav", false);

	Sounds["warn"] = new Sound();
	Sounds["warn"]->load("../assets/sounds/warning.wav", false);

	//SET BOUNDING BOXES

	SpotLight.BBBL = glm::vec2(-4.0f, 4.0f);
	SpotLight.BBFR = glm::vec2(4.0f, -4.0f);

	SpotLight2.BBBL = glm::vec2(-4.0f, 4.0f);
	SpotLight2.BBFR = glm::vec2(4.0f, -4.0f);

	QuadLight.BBBL = glm::vec2(-17.5f, 17.5f);
	QuadLight.BBFR = glm::vec2(17.5f, -17.5f);

	InvisWall.BBBL = glm::vec2(-50.0f, 1.0f);
	InvisWall.BBFR = glm::vec2(50.0f, -1.0f);

	InvisWall2.BBBL = glm::vec2(-50.0f, 1.0f);
	InvisWall2.BBFR = glm::vec2(50.0f, -1.0f);

	InvisWall3.BBBL = glm::vec2(-1.0f, 50.0f);
	InvisWall3.BBFR = glm::vec2(1.0f, -50.0f);

	InvisWall4.BBBL = glm::vec2(-1.0f, 50.0f);
	InvisWall4.BBFR = glm::vec2(1.0f, -50.0f);
	
	//SET POINTS
	points[0]  = glm::vec3(  0.0f, 0.5f,   0.0f);
	points[1]  = glm::vec3( 25.0f, 0.5f,  55.0f);
	points[2]  = glm::vec3(  0.0f, 0.5f,  55.0f);
	points[3]  = glm::vec3(-25.0f, 0.5f,  55.0f);
	points[4]  = glm::vec3(-55.0f, 0.5f,  25.0f);
	points[5]  = glm::vec3(-55.0f, 0.5f,   0.0f);
	points[6]  = glm::vec3(-55.0f, 0.5f, -25.0f);
	points[7]  = glm::vec3(-25.0f, 0.5f, -55.0f);
	points[8]  = glm::vec3(  0.0f, 0.5f, -55.0f);
	points[9]  = glm::vec3( 25.0f, 0.5f, -55.0f);
	points[10] = glm::vec3( 55.0f, 0.5f, -25.0f);
	points[11] = glm::vec3( 55.0f, 0.5f,  55.0f);
	points[12] = glm::vec3( 55.0f, 0.5f,  25.0f);
	points[13] = glm::vec3( 17.5f, 0.5f,  17.5f);
	points[14] = glm::vec3(-17.5f, 0.5f,  17.5f);
	points[15] = glm::vec3( 17.5f, 0.5f, -17.5f);
	points[16] = glm::vec3(-17.5f, 0.5f, -17.5f);
}

void Reset()
{
	//RESET ALL OBJECTS BEFORE REAPPLYING TRANSLATIONS AND BOOLEANS
	Player["Nyx"]->reset();

	for (sceneObjects_iterator = sceneObjects.begin(); sceneObjects_iterator != sceneObjects.end(); sceneObjects_iterator++)
	{
		sceneObjects_iterator->second->reset();
	}

	sceneObjects["SpotLight"]->setPosition(glm::vec3(0.0f, 0.6f, -55.0f));
	sceneObjects["SpotLight2"]->setPosition(glm::vec3(0.0f, 0.6f, -55.0f));

	globalT = 1.0f;
	rampValue = 0.005f;
}



MainMenu::MainMenu()
{
	hasBeenInitialized = false;
}

//MAIN MENU MAIN LOOP
void MainMenu::Update()
{
	if (hasBeenInitialized == false)
	{
		enter();
		hasBeenInitialized = true;
	}

	sceneObjects["Quad"]->uDiffuseMult = glm::vec3(1.0f, 1.0f, 1.0f);
	sceneObjects["Quad"]->uAmbientAdd = glm::vec3(1.0f, 1.0f, 1.0f);
	sceneObjects["Quad"]->uDiffuseMult = glm::vec3(sinf(totalTime) * 0.2f + 1.0f, sinf(totalTime) * 0.2f + 1.0f, sinf(totalTime) * 0.2f + 1.0f);

	totalTime += 1 / 60.0f;

	deltaTime = (totalTime - previousTime);

	previousTime = totalTime;

	passThrough.bind();

	gameWindow->update(defaultMesh, &passThrough, deltaTime);

	gameWindow->GetSFMLWindow()->display();

	if(devCommand.GetKeyDown(ENG::KeyCode::Space))
	{
		MainMenu::exit();
		m_parent->GetGameState("GameLevel")->SetPaused(false);
	}
}

void MainMenu::enter()
{
	if (m_paused == true)
	{
		m_paused = false;
		MainMenu::SetPaused(m_paused);
	}

	gameWindow->AddGameObject(sceneObjects["Quad"]);
	sceneObjects["Quad"]->setPosition(glm::vec3(0.0f, 0.0f, -10.0f));
	hasLoadedOnce = true;

}

void MainMenu::exit()
{
	gameWindow->RemoveGameObject(sceneObjects["Quad"]);

	hasBeenInitialized = false;
	if (m_paused == false)
	{
		m_paused = true;
		MainMenu::SetPaused(m_paused);
		GameLevel(hasBeenInitialized);
	}
}


//FIRST LEVEL MAIN LOOP
void GameLevel::Update()
{
	if (hasBeenInitialized == false)
	{
		enter();
		hasBeenInitialized = true;
	}

	//if (devCommand.GetKey(ENG::KeyCode::Right))
	//{
	//	view.rotateY(0.017453f);
	//}
	//if (devCommand.GetKey(ENG::KeyCode::Left))
	//{
	//	view.rotateY(-0.017453f);
	//}

	if (Player["Nyx"]->getLifeLost())
	{
		Player["Nyx"]->setLifeLost(false);
		Sounds["die"]->play();
		globalT = 0.99f;
	}

	if (globalT >= 1.0f)
	{
		globalT = 0.0f;

		Sounds["warn"]->play();
	
		randomLERPStart = randomNumber(1, 12);
		randomLERPEnd = randomLERPStart + randomNumber(4, 8);

		if (randomLERPEnd > 12)
		{
			randomLERPEnd = randomLERPStart / 2;
		}
	
		randomCurveStart = randomNumber(1, 12);
		randomCurveEnd = randomCurveStart + randomNumber(4, 8);
		randomCurveControl = randomNumber(0, 16);

		if (randomCurveEnd > 12)
		{
			randomCurveEnd = randomCurveStart / 2;
		}

		randomQuadPos = randomNumber(13, 16);
		
		wasWarned1 = false;
		wasWarned2 = false;

		rampValue += 0.00005f;
	}

	globalT += rampValue;

	totalTime += 1 / 60.0f;

	deltaTime = (totalTime - previousTime);
	
	previousTime = totalTime;

	defaultShader.sendUniform("uTime", totalTime);

	Player["Nyx"]->uDiffuseMult = glm::vec3(1.0f, 1.0f, 1.0f);
	sceneObjects["Room"]->uDiffuseMult = glm::vec3(2.0f, 2.0f, 2.0f);
	sceneObjects["SpotLight"]->uDiffuseMult = glm::vec3(1.0f, 1.0f, 1.0f);
	sceneObjects["SpotLight2"]->uDiffuseMult = glm::vec3(1.0f, 1.0f, 1.0f);
	sceneObjects["QuadLight"]->uDiffuseMult = glm::vec3(1.0f, 1.0f, 1.0f);

	sceneObjects["SpotLight"]->uEmissiveAdd = glm::vec3(sinf(globalT * 1.0f) * 0.25f, sinf(globalT * 1.0f) * 0.25f, sinf(globalT * 1.0f) * 0.25f);
	sceneObjects["SpotLight2"]->uEmissiveAdd = glm::vec3(sinf(globalT * 1.0f) * 0.25f, sinf(globalT * 1.0f) * 0.25f, sinf(globalT * 1.0f) * 0.25f);
	//sceneObjects["QuadLight"]->uEmissiveAdd = glm::vec3(sinf(globalT * 1.0f) * 0.25f, sinf(globalT * 1.0f) * 0.25f, sinf(globalT * 1.0f) * 0.25f);

	sceneObjects["SpotLight"]->setPosition(lerp(points[randomLERPStart], points[randomLERPEnd], globalT));
	sceneObjects["SpotLight2"]->setPosition(bezier(points[randomCurveStart], points[randomCurveControl], points[randomCurveEnd], globalT));

	if(globalT > 0.7f && globalT < 0.98f)
		sceneObjects["QuadLight"]->setPosition(points[randomQuadPos]);
	else
		sceneObjects["QuadLight"]->setPosition(glm::vec3(0.0f, 0.6f, 55.0f));

	if (!wasWarned1)
		sceneObjects["Warning"]->setPosition(clamp(sceneObjects["SpotLight"]->getPosition(), RoomMin, RoomMax));
	
	if (!wasWarned2)
		sceneObjects["Warning2"]->setPosition(clamp(sceneObjects["SpotLight2"]->getPosition(), RoomMin, RoomMax));

	if (globalT < 0.7f) 
		sceneObjects["Warning3"]->setPosition(points[randomQuadPos]);
	else
		sceneObjects["Warning3"]->setPosition(glm::vec3(0.0f, 0.6f, 55.0f));

	if (borderCheck(sceneObjects["SpotLight"]->getPosition(), RoomMin, RoomMax))
	{
		sceneObjects["Warning"]->setPosition(glm::vec3(0.0f, 0.0f, 55.0f));
		wasWarned1 = true;
	}

	if (borderCheck(sceneObjects["SpotLight2"]->getPosition(), RoomMin, RoomMax))
	{ 
		sceneObjects["Warning2"]->setPosition(glm::vec3(0.0f, 0.0f, 55.0f));
		wasWarned2 = true;
	}

	Player["Nyx"]->collisionCheck(collidables);

	defaultShader.bind();

	gameWindow->update(defaultMesh, &defaultShader, deltaTime);

	defaultShader.sendUniformMat4("uView", &view.getMatrix()[0][0], false);
	defaultShader.sendUniformMat4("uProj", &persp[0][0], false);
	defaultShader.sendUniform("LightPosition", down);

	gameWindow->GetSFMLWindow()->display();
	
	//if (Player["Nyx"]->getHasWon() == true)
	//{
	//	GameLevel::exit();
	//	m_parent->GetGameState("LevelTwo")->SetPaused(false);
	//}

	if (Player["Nyx"]->getIsDead() == true)
	{
		GameLevel::gameOver();
		m_parent->GetGameState("GameOver")->SetPaused(false);
	}
}

GameLevel::GameLevel()
{
	hasBeenInitialized = false;
}

void GameLevel::enter()
{
	//INITIALIZE
	if (m_paused == true)
	{
		m_paused = false;
		GameLevel::SetPaused(m_paused);
	}

	//POSITION OBJECTS
	Reset();

	Sounds["bgm"]->play();

	gameWindow->AddGameObject(Player["Nyx"]);
	Player["Nyx"]->setStartPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	Player["Nyx"]->setPosition(Player["Nyx"]->getStartPosition());
	//Player["Nyx"]->uInvisible = 0.4f;

	gameWindow->AddGameObject(sceneObjects["Room"]);
	sceneObjects["Room"]->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));

	gameWindow->AddGameObject(sceneObjects["SpotLight"]);
	sceneObjects["SpotLight"]->setLoss(true);
	sceneObjects["SpotLight"]->setPosition(glm::vec3(0.0f, 0.6f, -55.0f));

	gameWindow->AddGameObject(sceneObjects["SpotLight2"]);
	sceneObjects["SpotLight2"]->setLoss(true);
	sceneObjects["SpotLight2"]->setPosition(glm::vec3(0.0f, 0.6f, -55.0f));

	gameWindow->AddGameObject(sceneObjects["QuadLight"]);
	sceneObjects["QuadLight"]->setLoss(true);
	sceneObjects["QuadLight"]->setPosition(glm::vec3(0.0f, 0.6f, -55.0f));

	gameWindow->AddGameObject(sceneObjects["Warning"]);
	sceneObjects["Warning"]->setPosition(glm::vec3(0.0f, 10.0f, -55.0f));
	gameWindow->AddGameObject(sceneObjects["Warning2"]);
	sceneObjects["Warning2"]->setPosition(glm::vec3(0.0f, 10.0f, -55.0f));
	gameWindow->AddGameObject(sceneObjects["Warning3"]);
	sceneObjects["Warning3"]->setPosition(glm::vec3(0.0f, 10.0f, -55.0f));

	sceneObjects["InvisWall"]->setPosition(glm::vec3(0.0f, 0.0f, -35.0f));
	sceneObjects["InvisWall2"]->setPosition(glm::vec3(0.0f, 0.0f, 35.0f));
	sceneObjects["InvisWall3"]->setPosition(glm::vec3(-35.0f, 0.0f, 0.0f));
	sceneObjects["InvisWall4"]->setPosition(glm::vec3(35.0f, 0.0f, 0.0f));
	
	if (!hasLoadedOnce)
	{
		//PUSH BACK COLLIDABLE OBJECTS

		collidables.push_back(sceneObjects["SpotLight"]);
		collidables.push_back(sceneObjects["SpotLight2"]);
		collidables.push_back(sceneObjects["QuadLight"]);

		collidables.push_back(sceneObjects["InvisWall"]);
		collidables.push_back(sceneObjects["InvisWall2"]);
		collidables.push_back(sceneObjects["InvisWall3"]);
		collidables.push_back(sceneObjects["InvisWall4"]);

		hasLoadedOnce = true;
	}
}

//void GameLevel::exit()
//{
//	gameWindow->RemoveGameObject(Player["Nyx"]);
//	gameWindow->RemoveGameObject(sceneObjects["Room"]);
//	gameWindow->RemoveGameObject(sceneObjects["Fridge"]);
//	gameWindow->RemoveGameObject(sceneObjects["FirePlace"]);
//	gameWindow->RemoveGameObject(sceneObjects["Piano"]);
//	gameWindow->RemoveGameObject(sceneObjects["LoveSeat"]);
//	gameWindow->RemoveGameObject(sceneObjects["Chair"]);
//	gameWindow->RemoveGameObject(sceneObjects["Stairs"]);
//	gameWindow->RemoveGameObject(sceneObjects["Table"]);
//	gameWindow->RemoveGameObject(sceneObjects["FireplaceLight"]);
//	gameWindow->RemoveGameObject(sceneObjects["FridgeLight"]);
//	gameWindow->RemoveGameObject(sceneObjects["SpotLight"]);
//	gameWindow->RemoveGameObject(sceneObjects["SpotLight2"]);
//	gameWindow->RemoveGameObject(sceneObjects["SpotLight3"]);
//	gameWindow->RemoveGameObject(sceneObjects["Lamp"]);
//	gameWindow->RemoveGameObject(sceneObjects["Lamp2"]);
//	gameWindow->RemoveGameObject(sceneObjects["Lamp3"]);
//	gameWindow->RemoveGameObject(sceneObjects["ENGI"]);
//	gameWindow->RemoveGameObject(sceneObjects["ENGI2"]);
//
//	hasBeenInitialized = false;
//	if (m_paused == false)
//	{
//		m_paused = true;
//		GameLevel::SetPaused(m_paused);
//		LevelTwo(hasBeenInitialized);
//	}
//}

void GameLevel::gameOver()
{
	gameWindow->RemoveGameObject(Player["Nyx"]);
	gameWindow->RemoveGameObject(sceneObjects["Room"]);
	
	gameWindow->RemoveGameObject(sceneObjects["SpotLight"]);
	gameWindow->RemoveGameObject(sceneObjects["SpotLight2"]);
	gameWindow->RemoveGameObject(sceneObjects["QuadLight"]);

	gameWindow->RemoveGameObject(sceneObjects["Warning"]);
	gameWindow->RemoveGameObject(sceneObjects["Warning2"]);
	gameWindow->RemoveGameObject(sceneObjects["Warning3"]);

	Sounds["bgm"]->channel->stop();

	hasBeenInitialized = false;
	if (m_paused == false)
	{
		m_paused = true;
		GameLevel::SetPaused(m_paused);
		GameOver(hasBeenInitialized);
	}
}

//GAME OVER SCREEN MAIN LOOP
void GameOver::Update()
{
	if (hasBeenInitialized == false)
	{
		enter();
		hasBeenInitialized = true;
	}

	sceneObjects["Quad2"]->uDiffuseMult = glm::vec3(1.0f, 1.0f, 1.0f);
	sceneObjects["Quad2"]->uAmbientAdd = glm::vec3(1.0f, 1.0f, 1.0f);
	sceneObjects["Quad2"]->uDiffuseMult = glm::vec3(sinf(totalTime) * 0.2f + 1.0f, sinf(totalTime) * 0.2f + 1.0f, sinf(totalTime) * 0.2f + 1.0f);

	totalTime += 1 / 60.0f;

	deltaTime = (totalTime - previousTime);

	previousTime = totalTime;

	passThrough.bind();

	gameWindow->update(defaultMesh, &passThrough, deltaTime);

	defaultShader.sendUniformMat4("uView", &view.getMatrix()[0][0], false);
	defaultShader.sendUniformMat4("uProj", &persp[0][0], false);
	defaultShader.sendUniform("LightPosition", down);

	gameWindow->GetSFMLWindow()->display();

	if (devCommand.GetKeyDown(ENG::KeyCode::Space))
	{
		GameOver::exit();
		m_parent->GetGameState("MainMenu")->SetPaused(false);
	}
}

GameOver::GameOver()
{
	hasBeenInitialized = false;
}

void GameOver::enter()
{
	if (m_paused == true)
	{
		m_paused = false;
		GameOver::SetPaused(m_paused);
	}

	gameWindow->AddGameObject(sceneObjects["Quad2"]);
	sceneObjects["Quad2"]->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	hasLoadedOnce = true;
}

void GameOver::exit()
{
	gameWindow->RemoveGameObject(sceneObjects["Quad2"]);

	Reset();

	hasBeenInitialized = false;
	if (m_paused == false)
	{
		m_paused = true;
		GameOver::SetPaused(m_paused);
		MainMenu(hasBeenInitialized);
	}
}