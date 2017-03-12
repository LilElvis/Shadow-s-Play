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
	cameraPos = glm::vec3(0.0f, 0.0f, 85.0f);

	cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	cameraDirection = glm::normalize(cameraPos - cameraTarget);

	up = glm::vec3(0.0f, 1.0f, 0.0f);
	down = glm::vec4(0.0f, -1.0f, 0.0f, 1.0f);
	cameraRight = glm::normalize(glm::cross(up, cameraDirection));

	cameraUp = glm::cross(cameraDirection, cameraRight);

	cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	
	view.setMatrix(glm::lookAt(cameraPos, (cameraPos + cameraFront), up));
	view.rotateX(-1.57f);
	
	persp = glm::perspective(45.0f, 16.0f / 9.0f, 10.0f, 100.0f);

	glm::mat4 ProjBiasMatrix = glm::mat4(
		2.0f, 0.0f, 0.0f, -1.0f,
		0.0f, 2.0f, 0.0f, -1.0f,
		0.0f, 0.0f, 2.0f, -1.0f,
		0.0f, 0.0f, 0.0f,  1.0f);

	//LOAD DEFAULT TEXTURES
	defaultTexture->LoadFromFile("Emissive", "../assets/textures/default/Emissive.png");
	defaultTexture->LoadFromFile("Specular", "../assets/textures/default/Specular.png");
	defaultTexture->LoadFromFile("Normal", "../assets/textures/default/Normal.png");
	
	//LOAD MENU TEXTURES
	defaultMesh->LoadFromFile("Quad", "../assets/objects/Quad.obj");
	defaultTexture->LoadFromFile("MainMenu", "../assets/textures/Menu.png");
	defaultTexture->LoadFromFile("GameOver", "../assets/textures/GameOver.png");
	static ENG::SceneObject Quad("Quad", defaultMesh->listOfMeshes["Quad"]->VAO, *defaultTexture->listOfTextures["MainMenu"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], deferredFBO.getLayerNumber());
	sceneObjects["Quad"] = &Quad;
	static ENG::SceneObject Quad2("Quad", defaultMesh->listOfMeshes["Quad"]->VAO, *defaultTexture->listOfTextures["GameOver"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], deferredFBO.getLayerNumber());
	sceneObjects["Quad2"] = &Quad2;
	static ENG::SceneObject Quad3("Quad", defaultMesh->listOfMeshes["Quad"]->VAO, *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], deferredFBO.getLayerNumber());
	sceneObjects["Quad3"] = &Quad3;

	//LOAD OBJECTS
	defaultMesh->LoadFromFile("Nyx", "../assets/objects/Nyx/Nyx.obj");
	defaultTexture->LoadFromFile("Nyx", "../assets/textures/Nyx.png");
	defaultTexture->LoadFromFile("NyxEmissive", "../assets/textures/NyxEmissive.png");
	defaultTexture->LoadFromFile("NyxNormal", "../assets/textures/NyxNormal.png");
	static ENG::Player Nyx("Nyx", defaultMesh->listOfMeshes["Nyx"]->VAO, *defaultTexture->listOfTextures["Nyx"], *defaultTexture->listOfTextures["NyxNormal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["NyxEmissive"], deferredFBO.getLayerNumber());
	Player["Nyx"] = &Nyx;

	defaultMesh->LoadFromFile("Room", "../assets/objects/MapTemp.obj");
	defaultTexture->LoadFromFile("Room", "../assets/textures/Map.png");
	defaultTexture->LoadFromFile("RoomEmissive", "../assets/textures/MapEmissive.png");
	defaultTexture->LoadFromFile("RoomNormal", "../assets/textures/MapNormal.png");
	static ENG::SceneObject Room("Room", defaultMesh->listOfMeshes["Room"]->VAO, *defaultTexture->listOfTextures["Room"], *defaultTexture->listOfTextures["RoomNormal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["RoomEmissive"], deferredFBO.getLayerNumber());
	sceneObjects["Room"] = &Room;

	defaultMesh->LoadFromFile("Warning", "../assets/objects/Warning.obj");
	defaultTexture->LoadFromFile("Warning", "../assets/textures/Warning.png");
	static ENG::SceneObject Warning("Warning", defaultMesh->listOfMeshes["Warning"]->VAO, *defaultTexture->listOfTextures["Warning"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Warning"], deferredFBO.getLayerNumber());
	sceneObjects["Warning"] = &Warning;
	static ENG::SceneObject Warning2("Warning", defaultMesh->listOfMeshes["Warning"]->VAO, *defaultTexture->listOfTextures["Warning"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Warning"], deferredFBO.getLayerNumber());
	sceneObjects["Warning2"] = &Warning2;
	
	defaultMesh->LoadFromFile("SpotLight", "../assets/objects/SpotLight.obj");
	defaultTexture->LoadFromFile("SpotLight", "../assets/textures/Light1.png");
	defaultTexture->LoadFromFile("SpotLight2", "../assets/textures/Light2.png");
	static ENG::SceneObject SpotLight("SpotLight", defaultMesh->listOfMeshes["SpotLight"]->VAO, *defaultTexture->listOfTextures["SpotLight"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], deferredFBO.getLayerNumber());
	sceneObjects["SpotLight"] = &SpotLight;
	static ENG::SceneObject SpotLight2("SpotLight", defaultMesh->listOfMeshes["SpotLight"]->VAO, *defaultTexture->listOfTextures["SpotLight2"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], deferredFBO.getLayerNumber());
	sceneObjects["SpotLight2"] = &SpotLight2;

	defaultMesh->LoadFromFile("QuadLight", "../assets/objects/QuadLight.obj");
	defaultTexture->LoadFromFile("QuadLight", "../assets/textures/QuadLight.png");
	static ENG::SceneObject QuadLight("QuadLight", defaultMesh->listOfMeshes["QuadLight"]->VAO, *defaultTexture->listOfTextures["QuadLight"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], deferredFBO.getLayerNumber());
	sceneObjects["QuadLight"] = &QuadLight;
	defaultTexture->LoadFromFile("QuadWarning", "../assets/textures/QuadWarning.png");
	static ENG::SceneObject Warning3("Warning", defaultMesh->listOfMeshes["QuadLight"]->VAO, *defaultTexture->listOfTextures["QuadWarning"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["QuadWarning"], deferredFBO.getLayerNumber());
	sceneObjects["Warning3"] = &Warning3;

	static ENG::SceneObject InvisWall("InvisWall", 0, *defaultTexture->listOfTextures["SpotLight"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], 0);
	sceneObjects["InvisWall"] = &InvisWall;
	
	static ENG::SceneObject InvisWall2("InvisWall2", 0, *defaultTexture->listOfTextures["SpotLight"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], 0);
	sceneObjects["InvisWall2"] = &InvisWall2;
	
	static ENG::SceneObject InvisWall3("InvisWall3", 0, *defaultTexture->listOfTextures["SpotLight"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], 0);
	sceneObjects["InvisWall3"] = &InvisWall3;
	
	static ENG::SceneObject InvisWall4("InvisWall4", 0, *defaultTexture->listOfTextures["SpotLight"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], 0);
	sceneObjects["InvisWall4"] = &InvisWall4;

	//LOAD SHADER PROGRAMS
	defaultShader.load("Default", "../assets/shaders/StaticGeometry.vert", "../assets/shaders/BasicLighting.frag");
	passThrough.load("PassThrough", "../assets/shaders/PassThrough.vert", "../assets/shaders/PassThrough.frag");
	GBuffer.load("GBuffer", "../assets/shaders/StaticGeometry.vert", "../assets/shaders/GBuffer.frag");
	deferredShading.load("deferred", "../assets/shaders/PassThrough.vert", "../assets/shaders/DeferredLighting.frag");
	bloomHighPass.load("highPass", "../assets/shaders/PassThrough.vert", "../assets/shaders/HighPass.frag");
	bloomHorizontalBlur.load("horBlur", "../assets/shaders/PassThrough.vert", "../assets/shaders/HorizontalBlur.frag");
	bloomVerticalBlur.load("vertBlur", "../assets/shaders/PassThrough.vert", "../assets/shaders/VerticalBlur.frag");
	bloomComposite.load("bloomComp", "../assets/shaders/PassThrough.vert", "../assets/shaders/BloomComposite.frag");

	//INITIALiZE FBOS
	deferredFBO.Init(windowWidth, windowHeight, 5);
	deferredFBO.initColorTexture(0, GL_RGBA8);
	deferredFBO.initColorTexture(1, GL_RGB8);
	deferredFBO.initColorTexture(2, GL_RGB8);
	deferredFBO.initColorTexture(3, GL_RGB8);
	deferredFBO.initColorTexture(4, GL_RGB8);
	deferredFBO.initDepthTexture();

	finalSceneFBO1.Init(windowWidth, windowHeight, 1);
	finalSceneFBO1.initColorTexture(0, GL_RGB8);
	finalSceneFBO2.Init(windowWidth, windowHeight, 1);
	finalSceneFBO2.initColorTexture(0, GL_RGB8);

	bloomFBO1.Init(windowWidth/2, windowHeight/2, 1);
	bloomFBO1.initColorTexture(0);

	bloomFBO2.Init(windowWidth / 2, windowHeight / 2, 1);
	bloomFBO2.initColorTexture(0);

	//INITIALIZE/LOAD SOUNDS
	soundEngine.Init();
	Sounds["bgm"] = new Sound();
	Sounds["bgm"]->load("../assets/sounds/game_soundtrack.mp3", true, true);
	
	Sounds["die"] = new Sound();
	Sounds["die"]->load("../assets/sounds/die.wav", true, false);

	Sounds["warn"] = new Sound();
	Sounds["warn"]->load("../assets/sounds/warning.wav", true, false);

	Sounds["mainMenu"] = new Sound();
	Sounds["mainMenu"]->load("../assets/sounds/pause_menu.mp3", true, true);

	mainMenuChannel = NULL;
	bgmChannel = NULL;
	dieChannel = NULL;
	warningChannel = NULL;
	pauseMenuChannel = NULL;
	dashChannel = NULL;

	playerPos;
	playerVel;
	diePos;
	dieVel;
	warningPos;
	warningVel;

	pitchShift = NULL;
	lowPass = NULL;
	echo = NULL;

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
	timeOfDeath = 0.0f;
	deathTimer = false;
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

	gameWindow->checkAndClear();
	
	Sounds["mainMenu"]->channel->setVolume(0.6f);

	Sound::systemUpdate();
	
	passThrough.bind();

	for (auto itr = gObjects.begin(), itrEnd = gObjects.end();
		itr != itrEnd; itr++)
	{
		(*itr)->update(deltaTime, totalTime);
	}

	for (auto itr = gObjects.begin(), itrEnd = gObjects.end();
		itr != itrEnd; itr++)
	{
		(*itr)->render(defaultMesh, &passThrough);
	}

	gameWindow->GetSFMLWindow()->display();

	if(devCommand.GetKeyDown(ENG::KeyCode::Space))
	{
		MainMenu::exit();
		m_parent->GetGameState("GameLevel")->SetPaused(false);
		Sounds["mainMenu"]->channel->stop();
	}

	ENG::Input::ResetKeys();
}

void MainMenu::enter()
{
	if (m_paused == true)
	{
		m_paused = false;
		MainMenu::SetPaused(m_paused);
	}

	mainMenuChannel = Sounds["mainMenu"]->play();

	gObjects.push_back(sceneObjects["Quad"]);
	sceneObjects["Quad"]->setPosition(glm::vec3(0.0f, 0.0f, -10.0f));
	hasLoadedOnce = true;

}

void MainMenu::exit()
{
	removeGameObjects();

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
	timerFunc();

	if (!weBePausing)
	{
		if (hasBeenInitialized == false)
		{
			enter();
			hasBeenInitialized = true;
		}

		flashValue = (0.9f * sin(totalTime) + 0.55f);


		if (Player["Nyx"]->getLifeLost())
		{
			Player["Nyx"]->setLifeLost(false);
			dieChannel = Sounds["die"]->play();
			globalT = 0.99f;
		}

		if (globalT >= 1.0f)
		{
			globalT = 0.0f;

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

		sceneObjects["SpotLight"]->uEmissiveAdd = glm::vec3(sinf(globalT * 1.0f) * 0.25f, sinf(globalT * 1.0f) * 0.25f, sinf(globalT * 1.0f) * 0.25f);
		sceneObjects["SpotLight2"]->uEmissiveAdd = glm::vec3(sinf(globalT * 1.0f) * 0.25f, sinf(globalT * 1.0f) * 0.25f, sinf(globalT * 1.0f) * 0.25f);
		
		sceneObjects["SpotLight"]->setPosition(lerp(points[randomLERPStart], points[randomLERPEnd], globalT));
		pointLight.position = (sceneObjects["SpotLight"]->getPosition() + glm::vec3(0.0f, 1.5f, 0.0f));

		sceneObjects["SpotLight2"]->setPosition(bezier(points[randomCurveStart], points[randomCurveControl], points[randomCurveEnd], globalT));
		pointLight2.position = (sceneObjects["SpotLight2"]->getPosition() + glm::vec3(0.0f, 1.5f, 0.0f));

		if (globalT > 0.7f && globalT < 0.98f)
			sceneObjects["QuadLight"]->setPosition(points[randomQuadPos]);
		else
			sceneObjects["QuadLight"]->setPosition(glm::vec3(0.0f, 0.6f, 100.0f));

		pointLight3.position = (sceneObjects["QuadLight"]->getPosition() + glm::vec3(0.0f, 1.5f, 0.0f));

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

		sceneObjects["Warning"]->uEmissiveMult = glm::vec3((sinf(totalTime * 0.25f) * 0.5f) + 1.0f);
		sceneObjects["Warning2"]->uEmissiveMult = glm::vec3((sinf(totalTime * 0.25f) * 0.5f) + 1.0f);
		sceneObjects["Warning3"]->uEmissiveMult = glm::vec3((sinf(totalTime * 0.25f) * 0.5f) + 1.0f);

		Player["Nyx"]->collisionCheck(collidables);

		if (Player["Nyx"]->getIsDead() == true)
		{
			Player["Nyx"]->setPosition(glm::vec3(200.0f, 200.0f, 200.0f));
			if (deathTimer == false)
			{
				timeOfDeath = totalTime;
				deathTimer = true;
			}
			if ((deathTimer == true) && (totalTime > (timeOfDeath + 3.0f)))
			{
				GameLevel::gameOver();
				m_parent->GetGameState("GameOver")->SetPaused(false);
			}
		}
	
		//Sound Update

		Sounds["bgm"]->channel->setVolume(0.6f);
		Sounds["die"]->channel->setVolume(1.0f);
		Sounds["warn"]->channel->setVolume(25.0f);

		playerPos.x = Player["Nyx"]->getPosition().x;
		playerPos.y = Player["Nyx"]->getPosition().y;
		playerPos.z = Player["Nyx"]->getPosition().z;

		playerVel.x = Player["Nyx"]->getVelocity().x;
		playerVel.y = Player["Nyx"]->getVelocity().y;
		playerVel.z = Player["Nyx"]->getVelocity().z;

		diePos = playerPos;
		dieVel = playerVel;

		warningPos.x = sceneObjects["Warning3"]->getPosition().x;
		warningPos.y = sceneObjects["Warning3"]->getPosition().y;
		warningPos.z = sceneObjects["Warning3"]->getPosition().z;

		warningVel.x = sceneObjects["Warning3"]->getVelocity().x;
		warningVel.y = sceneObjects["Warning3"]->getVelocity().y;
		warningVel.z = sceneObjects["Warning3"]->getVelocity().z;

		Sounds["bgm"]->setPosition(bgmChannel, playerPos, playerVel);
		Sounds["die"]->setPosition(dieChannel, diePos, dieVel);
		Sounds["warn"]->setPosition(warningChannel, warningPos, warningVel);
		
		Sound::Sys.listenerPos = playerPos;
		Sound::Sys.listenerVel = playerVel;
		
		Sound::systemUpdate();

		if (glm::distance(Player["Nyx"]->getPosition(), sceneObjects["Warning3"]->getPosition()) <= 19.0f && !inRadius)
			hasBeenWarned = true;

		if (glm::distance(Player["Nyx"]->getPosition(), sceneObjects["Warning3"]->getPosition()) <= 19.0f)
			inRadius = true;
		else
			inRadius = false;
		
		if (hasBeenWarned)
		{
			warningChannel = Sounds["warn"]->play();
			hasBeenWarned = false;
		}

		deferredFBO.Bind();

		GBuffer.bind();

		GBuffer.sendUniformMat4("uView", &view.getMatrix()[0][0], false);
		GBuffer.sendUniformMat4("uProj", &persp[0][0], false);
		GBuffer.sendUniform("LightPosition", down);
		
		gameWindow->checkAndClear();

		for (auto itr = gObjects.begin(), itrEnd = gObjects.end();
			itr != itrEnd; itr++)
		{
			(*itr)->update(deltaTime, totalTime);
		}

		for (auto itr = gObjects.begin(), itrEnd = gObjects.end();
			itr != itrEnd; itr++)
		{
			(*itr)->render(defaultMesh, &GBuffer);
		}

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		defaultShader.bind();

		defaultShader.sendUniformMat4("uView", &view.getMatrix()[0][0], false);
		defaultShader.sendUniformMat4("uProj", &persp[0][0], false);
		defaultShader.sendUniform("LightPosition", down);

		for (auto itr = transparentGObjects.begin(), itrEnd = transparentGObjects.end();
			itr != itrEnd; itr++)
		{
			(*itr)->update(deltaTime, totalTime);
		}

		for (auto itr = transparentGObjects.begin(), itrEnd = transparentGObjects.end();
			itr != itrEnd; itr++)
		{
			(*itr)->render(defaultMesh, &defaultShader);
		}

		defaultShader.unBind();

		glDisable(GL_BLEND);

		GBuffer.unBind();
		deferredFBO.Unbind();
		
		finalSceneFBO1.Bind();
		deferredShading.bind();

		viewInverse = glm::inverse(view.getMatrix());

		deferredShading.sendUniformMat4("uinverseViewMatrix", &viewInverse[0][0], false);
		deferredShading.sendUniformMat4("uinversePerspectiveMatrix", &glm::inverse(persp)[0][0], false);

		updatedCamPos = viewInverse[3];
		deferredShading.sendUniform("uCameraPos", updatedCamPos);
		
		deferredShading.sendUniform("uNumLights", 3);
		deferredShading.sendUniformPointLight("lights", &pointLight, 0);
		deferredShading.sendUniformPointLight("lights", &pointLight2, 1);
		deferredShading.sendUniformPointLight("lights", &pointLight3, 2);

		glBindVertexArray(sceneObjects["Quad3"]->getRenderable());

		deferredFBO.BindColorAsTexture(GL_TEXTURE0, 0);
		deferredFBO.BindColorAsTexture(GL_TEXTURE1, 1);
		deferredFBO.BindColorAsTexture(GL_TEXTURE2, 2);
		deferredFBO.BindColorAsTexture(GL_TEXTURE3, 3);
		deferredFBO.BindColorAsTexture(GL_TEXTURE4, 4);
		deferredFBO.BindDepthAsTexture(GL_TEXTURE5);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindVertexArray(0);

		deferredShading.unBind();
		glViewport(0, 0, windowWidth / 2, windowHeight / 2);
		finalSceneFBO1.Unbind();

		bloomFBO1.Bind();
		bloomHighPass.bind();
		
		bloomHighPass.sendUniform("uScene", 0);
		
		glBindVertexArray(sceneObjects["Quad3"]->getRenderable());
		
		finalSceneFBO1.BindColorAsTexture(GL_TEXTURE0, 0);
		
		glDrawArrays(GL_TRIANGLES, 0, 6);
		
		glBindVertexArray(0);
		
		bloomHighPass.unBind();
		bloomFBO1.Unbind();
		
		for (int i = 0; i < int(sinf(totalTime *2.0f) * 10 + 11); i++)
		{
			bloomFBO2.Bind();
			bloomHorizontalBlur.bind();

			bloomHorizontalBlur.sendUniform("texelWidth", (2.0f / windowWidth));

			glBindVertexArray(sceneObjects["Quad3"]->getRenderable());

			bloomFBO1.BindColorAsTexture(GL_TEXTURE0, 0);

			glDrawArrays(GL_TRIANGLES, 0, 6);

			glBindVertexArray(0);

			bloomHorizontalBlur.unBind();
			bloomFBO2.Unbind();

			bloomFBO1.Bind();
			bloomVerticalBlur.bind();

			bloomVerticalBlur.sendUniform("texelHeight", (2.0f / windowHeight));

			glBindVertexArray(sceneObjects["Quad3"]->getRenderable());

			bloomFBO2.BindColorAsTexture(GL_TEXTURE0, 0);

			glDrawArrays(GL_TRIANGLES, 0, 6);

			glBindVertexArray(0);

			bloomVerticalBlur.unBind();
			bloomFBO1.Unbind();
		}

		glViewport(0, 0, windowWidth, windowHeight);
		finalSceneFBO2.Bind();
		bloomComposite.bind();
		bloomComposite.sendUniform("uScene", 0);
		
		finalSceneFBO1.BindColorAsTexture(GL_TEXTURE0, 0);
		
		bloomComposite.sendUniform("uBloomScene", 1);
		
		bloomFBO1.BindColorAsTexture(GL_TEXTURE1, 0);
		
		glBindVertexArray(sceneObjects["Quad3"]->getRenderable());
		
		glDrawArrays(GL_TRIANGLES, 0, 6);
		
		glBindVertexArray(0);
		
		bloomComposite.unBind();
		finalSceneFBO2.Unbind();

		//bloomFBO1.DrawToBackBuffer();
		finalSceneFBO2.DrawToBackBuffer();
		
	}

	if (devCommand.GetKeyDown(ENG::KeyCode::P) || sf::Joystick::isButtonPressed(0, 7))
	{
		weBePausing = !weBePausing;
		if (weBePausing)
		{
			Player["Nyx"]->setNyxPaused(true);
		}
		else
		{
			Player["Nyx"]->setNyxPaused(false);
		}
	}

	gameWindow->GetSFMLWindow()->display();

	//Sound::systemUpdate();
	ENG::Input::ResetKeys();
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

	bgmChannel = Sounds["bgm"]->play();

	gObjects.push_back(Player["Nyx"]);
	Player["Nyx"]->setStartPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	Player["Nyx"]->setPosition(Player["Nyx"]->getStartPosition());

	gObjects.push_back(sceneObjects["Room"]);
	sceneObjects["Room"]->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));

	gObjects.push_back(sceneObjects["SpotLight"]);
	sceneObjects["SpotLight"]->setLoss(true);
	sceneObjects["SpotLight"]->setPosition(glm::vec3(0.0f, 0.6f, -55.0f));

	gObjects.push_back(sceneObjects["SpotLight2"]);
	sceneObjects["SpotLight2"]->setLoss(true);
	sceneObjects["SpotLight2"]->setPosition(glm::vec3(0.0f, 0.6f, -55.0f));

	gObjects.push_back(sceneObjects["QuadLight"]);
	sceneObjects["QuadLight"]->setLoss(true);
	sceneObjects["QuadLight"]->setPosition(glm::vec3(0.0f, 0.6f, -55.0f));

	transparentGObjects.push_back(sceneObjects["Warning3"]);
	sceneObjects["Warning3"]->setPosition(glm::vec3(0.0f, 10.0f, -55.0f));
	transparentGObjects.push_back(sceneObjects["Warning"]);
	sceneObjects["Warning"]->setPosition(glm::vec3(0.0f, 10.0f, -55.0f));
	transparentGObjects.push_back(sceneObjects["Warning2"]);
	sceneObjects["Warning2"]->setPosition(glm::vec3(0.0f, 10.0f, -55.0f));
	
	sceneObjects["InvisWall"]->setPosition(glm::vec3(0.0f, 0.0f, -35.0f));
	sceneObjects["InvisWall2"]->setPosition(glm::vec3(0.0f, 0.0f, 35.0f));
	sceneObjects["InvisWall3"]->setPosition(glm::vec3(-35.0f, 0.0f, 0.0f));
	sceneObjects["InvisWall4"]->setPosition(glm::vec3(35.0f, 0.0f, 0.0f));

	Player["Nyx"]->uDiffuseAdd = glm::vec3(0.0f, 0.0f, 0.0f);
	Player["Nyx"]->uDiffuseMult = glm::vec3(1.0f, 1.0f, 1.0f);
	Player["Nyx"]->uSpecularAdd = glm::vec3(0.0f, 0.0f, 0.0f);
	Player["Nyx"]->uSpecularMult = glm::vec3(1.0f, 1.0f, 1.0f);
	Player["Nyx"]->uAmbientAdd = glm::vec3(0.2f, 0.2f, 0.2f);
	Player["Nyx"]->uAmbientMult = glm::vec3(1.0f, 1.0f, 1.0f);
	Player["Nyx"]->uEmissiveAdd = glm::vec3(0.2f, 0.1f, 0.2f);
	Player["Nyx"]->uEmissiveMult = glm::vec3(1.0f, 1.0f, 1.0f);

	sceneObjects["Room"]->uDiffuseMult = glm::vec3(1.0f, 1.0f, 1.0f);
	sceneObjects["Room"]->uAmbientAdd = glm::vec3(0.2f, 0.2f, 0.2f);
	sceneObjects["Room"]->uSpecularMult = glm::vec3(1.0f, 1.0f, 1.0f);
	sceneObjects["Room"]->uSpecularAdd = glm::vec3(0.0f, 0.0f, 0.0f);

	//sceneObjects["Warning"]->uDiffuseAdd = glm::vec3(1.0f, 1.0f, 1.0f);
	//sceneObjects["Warning2"]->uDiffuseAdd = glm::vec3(1.0f, 1.0f, 1.0f);
	//sceneObjects["Warning3"]->uDiffuseAdd = glm::vec3(1.0f, 1.0f, 1.0f);
	sceneObjects["Warning"]->uAmbientMult = glm::vec3(4.0f, 4.0f, 4.0f);
	sceneObjects["Warning2"]->uAmbientMult = glm::vec3(4.0f, 4.0f, 4.0f);
	sceneObjects["Warning3"]->uAmbientMult = glm::vec3(4.0f, 4.0f, 4.0f);

	sceneObjects["SpotLight"]->uDiffuseMult = glm::vec3(1.0f, 1.0f, 1.0f);
	sceneObjects["SpotLight2"]->uDiffuseMult = glm::vec3(1.0f, 1.0f, 1.0f);
	sceneObjects["QuadLight"]->uDiffuseMult = glm::vec3(1.0f, 1.0f, 1.0f);

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

		pointLight.position = sceneObjects["SpotLight"]->getPosition();
		pointLight.color = glm::vec4(1.0f, 0.63f, 0.72f, 1.0f);

		pointLight2.position = sceneObjects["SpotLight2"]->getPosition();
		pointLight2.color = glm::vec4(0.48f, 0.41f, 0.93f, 1.0f);

		pointLight3.position = sceneObjects["QuadLight"]->getPosition();
		pointLight3.color = glm::vec4(0.3f, 0.7f, 0.4f, 1.0f);
		pointLight3.aConstant = pointLight3.aConstant * 0.1f;
		pointLight3.aLinear = pointLight3.aLinear * 0.1f;
		pointLight3.aQuadratic = pointLight3.aQuadratic * 0.1f;

		hasLoadedOnce = true;
	}
}

void GameLevel::gameOver()
{
	removeGameObjects();

	Sounds["bgm"]->channel->stop();
	Sounds["die"]->channel->stop();
	Sounds["warn"]->channel->stop();

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

	gameWindow->checkAndClear();

	for (auto itr = gObjects.begin(), itrEnd = gObjects.end();
		itr != itrEnd; itr++)
	{
		(*itr)->update(deltaTime, totalTime);
	}

	for (auto itr = gObjects.begin(), itrEnd = gObjects.end();
		itr != itrEnd; itr++)
	{
		(*itr)->render(defaultMesh, &passThrough);
	}

	gameWindow->GetSFMLWindow()->display();

	if (devCommand.GetKeyDown(ENG::KeyCode::Space))
	{
		GameOver::exit();
		m_parent->GetGameState("MainMenu")->SetPaused(false);
	}

	ENG::Input::ResetKeys();
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

	gObjects.push_back(sceneObjects["Quad2"]);
	sceneObjects["Quad2"]->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	hasLoadedOnce = true;
}

void GameOver::exit()
{
	removeGameObjects();

	Reset();

	hasBeenInitialized = false;
	if (m_paused == false)
	{
		m_paused = true;
		GameOver::SetPaused(m_paused);
		MainMenu(hasBeenInitialized);
	}
}

float timerFunc()
	{
		seconds += 0.01666666666666;

		if (seconds >= 60.0f)
		{
			seconds = 0.0f;
			minute += 1;
		}

		//std::cout << minute << " : " << seconds << std::endl;
		return seconds;

	}

void removeGameObjects()
{
	gObjects.clear();
	transparentGObjects.clear();
}