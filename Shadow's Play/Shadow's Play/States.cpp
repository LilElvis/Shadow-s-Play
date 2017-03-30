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

	//LOAD SHADER PROGRAMS
	defaultShader.load("Default", "../assets/shaders/StaticGeometry.vert", "../assets/shaders/BasicLighting.frag");
	passThrough.load("PassThrough", "../assets/shaders/PassThrough.vert", "../assets/shaders/PassThrough.frag");
	GBuffer.load("GBuffer", "../assets/shaders/StaticGeometry.vert", "../assets/shaders/GBuffer.frag");
	lightingComposite.load("lightComp", "../assets/shaders/PassThrough.vert", "../assets/shaders/LightComposite.frag");
	deferredLighting.load("lighting", "../assets/shaders/StaticGeometry.vert", "../assets/shaders/DeferredLighting.frag");
	bloomHighPass.load("highPass", "../assets/shaders/PassThrough.vert", "../assets/shaders/HighPass.frag");
	bloomHorizontalBlur.load("horBlur", "../assets/shaders/PassThrough.vert", "../assets/shaders/HorizontalBlur.frag");
	bloomVerticalBlur.load("vertBlur", "../assets/shaders/PassThrough.vert", "../assets/shaders/VerticalBlur.frag");
	bloomComposite.load("bloomComp", "../assets/shaders/PassThrough.vert", "../assets/shaders/BloomComposite.frag");
	UVScrolling.load("UVScroll", "../assets/shaders/StaticGeometry.vert", "../assets/shaders/UVScrolling.frag");
	motionBlur.load("PassThrough", "../assets/shaders/PassThrough.vert", "../assets/shaders/MotionBlur.frag");
	colorCorrect.load("ColorCorrect", "../assets/shaders/PassThrough.vert", "../assets/shaders/LUT.frag");
	
	//LOD THINGS ONLY NECESSARY FOR LOADING SCREEN
	defaultTexture->LoadFromFile("Logo", "../assets/textures/Logo.png");
	
	defaultMesh->LoadFromFile("Quad", "../assets/objects/Quad.obj");
	static ENG::SceneObject Quad3("Quad", defaultMesh->listOfMeshes["Quad"]->VAO, *defaultTexture->listOfTextures["Logo"], *defaultTexture->listOfTextures["Logo"], *defaultTexture->listOfTextures["Logo"], *defaultTexture->listOfTextures["Logo"], geometryBuffer.getLayerNumber());
	sceneObjects["Quad3"] = &Quad3;

	passThrough.bind();
	defaultTexture->listOfTextures["Logo"]->Bind(GL_TEXTURE0);
	passThrough.sendUniform("uTex", 0);

	glBindVertexArray(sceneObjects["Quad3"]->getRenderable());
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	passThrough.unBind();

	gameWindow->GetSFMLWindow()->display();
	
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

	//orthoView = glm::mat4();
	float orthoSize = 5.0f;
	orthoPersp = glm::ortho(-orthoSize * aspectRatio, orthoSize * aspectRatio, -orthoSize, orthoSize, -100.0f, 100.0f);

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
	defaultTexture->LoadFromFile("MainMenu", "../assets/textures/Menu.png");
	defaultTexture->LoadFromFile("GameOver", "../assets/textures/GameOver.png");
	static ENG::SceneObject Quad("Quad", defaultMesh->listOfMeshes["Quad"]->VAO, *defaultTexture->listOfTextures["MainMenu"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], geometryBuffer.getLayerNumber());
	sceneObjects["Quad"] = &Quad;
	static ENG::SceneObject Quad2("Quad", defaultMesh->listOfMeshes["Quad"]->VAO, *defaultTexture->listOfTextures["GameOver"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], geometryBuffer.getLayerNumber());
	sceneObjects["Quad2"] = &Quad2;

	//LOAD OBJECTS
	defaultMesh->LoadFromFile("Nyx", "../assets/objects/Nyx/Nyx.obj");
	defaultTexture->LoadFromFile("Nyx", "../assets/textures/Nyx.png");
	defaultTexture->LoadFromFile("NyxEmissive", "../assets/textures/NyxEmissive.png");
	defaultTexture->LoadFromFile("NyxNormal", "../assets/textures/NyxNormal.png");
	defaultTexture->LoadFromFile("NyxSpecular", "../assets/textures/NyxSpecular.png");
	static ENG::Player Nyx("Nyx", defaultMesh->listOfMeshes["Nyx"]->VAO, *defaultTexture->listOfTextures["Nyx"], *defaultTexture->listOfTextures["NyxNormal"], *defaultTexture->listOfTextures["NyxSpecular"], *defaultTexture->listOfTextures["NyxEmissive"], geometryBuffer.getLayerNumber());
	Player["Nyx"] = &Nyx;

	defaultMesh->LoadFromFile("Room", "../assets/objects/MapTemp.obj");
	defaultTexture->LoadFromFile("Room", "../assets/textures/Map.png");
	defaultTexture->LoadFromFile("RoomEmissive", "../assets/textures/MapEmissive.png");
	defaultTexture->LoadFromFile("RoomNormal", "../assets/textures/MapNormal.png");
	static ENG::SceneObject Room("Room", defaultMesh->listOfMeshes["Room"]->VAO, *defaultTexture->listOfTextures["Room"], *defaultTexture->listOfTextures["RoomNormal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["RoomEmissive"], geometryBuffer.getLayerNumber());
	sceneObjects["Room"] = &Room;

	defaultMesh->LoadFromFile("Warning", "../assets/objects/WarnQuad.obj");
	defaultTexture->LoadFromFile("Warning", "../assets/textures/Warning.png");
	static ENG::SceneObject Warning("Warning", defaultMesh->listOfMeshes["Warning"]->VAO, *defaultTexture->listOfTextures["Warning"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Warning"], geometryBuffer.getLayerNumber());
	sceneObjects["Warning"] = &Warning;
	static ENG::SceneObject Warning2("Warning", defaultMesh->listOfMeshes["Warning"]->VAO, *defaultTexture->listOfTextures["Warning"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Warning"], geometryBuffer.getLayerNumber());
	sceneObjects["Warning2"] = &Warning2;
	
	defaultMesh->LoadFromFile("SpotLight", "../assets/objects/SpotLight.obj");
	defaultTexture->LoadFromFile("SpotLight", "../assets/textures/Light1.png");
	defaultTexture->LoadFromFile("SpotLight2", "../assets/textures/Light2.png");
	static ENG::SceneObject SpotLight("SpotLight", 0, *defaultTexture->listOfTextures["SpotLight"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], geometryBuffer.getLayerNumber());
	sceneObjects["SpotLight"] = &SpotLight;
	static ENG::SceneObject SpotLight2("SpotLight", 0, *defaultTexture->listOfTextures["SpotLight2"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], geometryBuffer.getLayerNumber());
	sceneObjects["SpotLight2"] = &SpotLight2;

	defaultMesh->LoadFromFile("QuadLight", "../assets/objects/QuadLight.obj");
	defaultTexture->LoadFromFile("QuadLight", "../assets/textures/QuadLight.png");
	static ENG::SceneObject QuadLight("QuadLight", 0, *defaultTexture->listOfTextures["QuadLight"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], geometryBuffer.getLayerNumber());
	sceneObjects["QuadLight"] = &QuadLight;

	defaultMesh->LoadFromFile("TriLight", "../assets/objects/TriLight.obj");
	defaultTexture->LoadFromFile("TriLight", "../assets/textures/QuadLight.png");
	static ENG::SceneObject TriLight("TriLight", 0, *defaultTexture->listOfTextures["QuadLight"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], geometryBuffer.getLayerNumber());
	sceneObjects["TriLight"] = &TriLight;
	static ENG::SceneObject TriLight2("TriLight", 0, *defaultTexture->listOfTextures["QuadLight"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], geometryBuffer.getLayerNumber());
	sceneObjects["TriLight2"] = &TriLight2;
	
	defaultMesh->LoadFromFile("QuadWarning", "../assets/objects/QuadWarningQuad.obj");
	defaultTexture->LoadFromFile("QuadWarning", "../assets/textures/QuadWarning.png");
	static ENG::SceneObject Warning3("Warning", defaultMesh->listOfMeshes["QuadWarning"]->VAO, *defaultTexture->listOfTextures["QuadWarning"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["QuadWarning"], geometryBuffer.getLayerNumber());
	sceneObjects["Warning3"] = &Warning3;
	static ENG::SceneObject Warning4("Warning", defaultMesh->listOfMeshes["TriLight"]->VAO, *defaultTexture->listOfTextures["QuadWarning"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["QuadWarning"], geometryBuffer.getLayerNumber());
	sceneObjects["Warning4"] = &Warning4;
	static ENG::SceneObject Warning5("Warning", defaultMesh->listOfMeshes["TriLight"]->VAO, *defaultTexture->listOfTextures["QuadWarning"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["QuadWarning"], geometryBuffer.getLayerNumber());
	sceneObjects["Warning5"] = &Warning5;

	defaultMesh->LoadFromFile("Candle", "../assets/objects/Candle.obj");
	defaultTexture->LoadFromFile("Candle", "../assets/textures/Candle.png");
	static ENG::SceneObject Candle("Candle", defaultMesh->listOfMeshes["Candle"]->VAO, *defaultTexture->listOfTextures["Candle"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Candle"], geometryBuffer.getLayerNumber());
	sceneObjects["Candle"] = &Candle;
	static ENG::SceneObject Candle2("Candle", defaultMesh->listOfMeshes["Candle"]->VAO, *defaultTexture->listOfTextures["Candle"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Candle"], geometryBuffer.getLayerNumber());
	sceneObjects["Candle2"] = &Candle2;
	static ENG::SceneObject Candle3("Candle", defaultMesh->listOfMeshes["Candle"]->VAO, *defaultTexture->listOfTextures["Candle"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Candle"], geometryBuffer.getLayerNumber());
	sceneObjects["Candle3"] = &Candle3;
	static ENG::SceneObject Candle4("Candle", defaultMesh->listOfMeshes["Candle"]->VAO, *defaultTexture->listOfTextures["Candle"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Candle"], geometryBuffer.getLayerNumber());
	sceneObjects["Candle4"] = &Candle4;
	static ENG::SceneObject Candle5("Candle", defaultMesh->listOfMeshes["Candle"]->VAO, *defaultTexture->listOfTextures["Candle"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Candle"], geometryBuffer.getLayerNumber());
	sceneObjects["Candle5"] = &Candle5;
	static ENG::SceneObject Candle6("Candle", defaultMesh->listOfMeshes["Candle"]->VAO, *defaultTexture->listOfTextures["Candle"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Candle"], geometryBuffer.getLayerNumber());
	sceneObjects["Candle6"] = &Candle6;
	static ENG::SceneObject Candle7("Candle", defaultMesh->listOfMeshes["Candle"]->VAO, *defaultTexture->listOfTextures["Candle"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Candle"], geometryBuffer.getLayerNumber());
	sceneObjects["Candle7"] = &Candle7;
	static ENG::SceneObject Candle8("Candle", defaultMesh->listOfMeshes["Candle"]->VAO, *defaultTexture->listOfTextures["Candle"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Candle"], geometryBuffer.getLayerNumber());
	sceneObjects["Candle8"] = &Candle8;

	defaultMesh->LoadFromFile("TimerQuad", "../assets/objects/TimerQuad.obj");
	defaultTexture->LoadFromFile("numTexture", "../assets/textures/numTexture.png");
	static ENG::SceneObject TimerQuad("TimerQuad", defaultMesh->listOfMeshes["TimerQuad"]->VAO, *defaultTexture->listOfTextures["numTexture"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], geometryBuffer.getLayerNumber());
	sceneObjects["TimerQuad"] = &TimerQuad;
	static ENG::SceneObject TimerQuad2("TimerQuad", defaultMesh->listOfMeshes["TimerQuad"]->VAO, *defaultTexture->listOfTextures["numTexture"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], geometryBuffer.getLayerNumber());
	sceneObjects["TimerQuad2"] = &TimerQuad2;
	static ENG::SceneObject TimerQuad3("TimerQuad", defaultMesh->listOfMeshes["TimerQuad"]->VAO, *defaultTexture->listOfTextures["numTexture"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], geometryBuffer.getLayerNumber());
	sceneObjects["TimerQuad3"] = &TimerQuad3;
	static ENG::SceneObject TimerQuad4("TimerQuad", defaultMesh->listOfMeshes["TimerQuad"]->VAO, *defaultTexture->listOfTextures["numTexture"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], geometryBuffer.getLayerNumber());
	sceneObjects["TimerQuad4"] = &TimerQuad4;
	static ENG::SceneObject TimerQuad5("TimerQuad", defaultMesh->listOfMeshes["TimerQuad"]->VAO, *defaultTexture->listOfTextures["numTexture"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], geometryBuffer.getLayerNumber());
	sceneObjects["TimerQuad5"] = &TimerQuad5;
	
	static ENG::SceneObject GameOverY("TimerQuad", defaultMesh->listOfMeshes["TimerQuad"]->VAO, *defaultTexture->listOfTextures["numTexture"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], geometryBuffer.getLayerNumber());
	sceneObjects["GameOverY"] = &GameOverY;
	static ENG::SceneObject GameOverO("TimerQuad", defaultMesh->listOfMeshes["TimerQuad"]->VAO, *defaultTexture->listOfTextures["numTexture"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], geometryBuffer.getLayerNumber());
	sceneObjects["GameOverO"] = &GameOverO;
	static ENG::SceneObject GameOverU("TimerQuad", defaultMesh->listOfMeshes["TimerQuad"]->VAO, *defaultTexture->listOfTextures["numTexture"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], geometryBuffer.getLayerNumber());
	sceneObjects["GameOverU"] = &GameOverU;
	static ENG::SceneObject GameOverD("TimerQuad", defaultMesh->listOfMeshes["TimerQuad"]->VAO, *defaultTexture->listOfTextures["numTexture"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], geometryBuffer.getLayerNumber());
	sceneObjects["GameOverD"] = &GameOverD;
	static ENG::SceneObject GameOverI("TimerQuad", defaultMesh->listOfMeshes["TimerQuad"]->VAO, *defaultTexture->listOfTextures["numTexture"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], geometryBuffer.getLayerNumber());
	sceneObjects["GameOverI"] = &GameOverI;
	static ENG::SceneObject GameOverE("TimerQuad", defaultMesh->listOfMeshes["TimerQuad"]->VAO, *defaultTexture->listOfTextures["numTexture"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], geometryBuffer.getLayerNumber());
	sceneObjects["GameOverE"] = &GameOverE;
	static ENG::SceneObject GameOverD2("TimerQuad", defaultMesh->listOfMeshes["TimerQuad"]->VAO, *defaultTexture->listOfTextures["numTexture"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], geometryBuffer.getLayerNumber());
	sceneObjects["GameOverD2"] = &GameOverD2;


	defaultMesh->LoadFromFile("HUD", "../assets/objects/StatsQuad.obj");
	defaultTexture->LoadFromFile("Hearts", "../assets/textures/Hearts.png");
	defaultTexture->LoadFromFile("Dash", "../assets/textures/Dash.png");
	static ENG::SceneObject HUD("HUD", defaultMesh->listOfMeshes["HUD"]->VAO, *defaultTexture->listOfTextures["Hearts"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], geometryBuffer.getLayerNumber());
	sceneObjects["HUD"] = &HUD;
	static ENG::SceneObject HUD2("HUD", defaultMesh->listOfMeshes["HUD"]->VAO, *defaultTexture->listOfTextures["Dash"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], geometryBuffer.getLayerNumber());
	sceneObjects["HUD2"] = &HUD2;

	defaultMesh->LoadFromFile("NumPlate", "../assets/objects/NumPlate.obj");
	defaultTexture->LoadFromFile("NumPlate", "../assets/textures/NumPlate.png");
	static ENG::SceneObject NumPlate("NumPlate", defaultMesh->listOfMeshes["NumPlate"]->VAO, *defaultTexture->listOfTextures["NumPlate"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], geometryBuffer.getLayerNumber());
	sceneObjects["NumPlate"] = &NumPlate;

	static ENG::SceneObject InvisWall("InvisWall", 0, *defaultTexture->listOfTextures["SpotLight"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], 0);
	sceneObjects["InvisWall"] = &InvisWall;
	
	static ENG::SceneObject InvisWall2("InvisWall2", 0, *defaultTexture->listOfTextures["SpotLight"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], 0);
	sceneObjects["InvisWall2"] = &InvisWall2;
	
	static ENG::SceneObject InvisWall3("InvisWall3", 0, *defaultTexture->listOfTextures["SpotLight"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], 0);
	sceneObjects["InvisWall3"] = &InvisWall3;
	
	static ENG::SceneObject InvisWall4("InvisWall4", 0, *defaultTexture->listOfTextures["SpotLight"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], 0);
	sceneObjects["InvisWall4"] = &InvisWall4;

	//LIGHT OBJECTS
	defaultMesh->LoadFromFile("Sphere", "../assets/objects/light_volumes/sphere.obj");
	defaultMesh->LoadFromFile("Cube", "../assets/objects/light_volumes/cube.obj");
	defaultMesh->LoadFromFile("Cone", "../assets/objects/light_volumes/ConeLight.obj");
	defaultMesh->LoadFromFile("Pyramid", "../assets/objects/light_volumes/PyramidLight.obj");

	pointLight.renderVolume = defaultMesh->listOfMeshes["Cone"]->VAO;
	pointLight2.renderVolume = defaultMesh->listOfMeshes["Cone"]->VAO;
	pointLight3.renderVolume = defaultMesh->listOfMeshes["Pyramid"]->VAO;
	pointLight4.renderVolume = defaultMesh->listOfMeshes["Pyramid"]->VAO;
	pointLight5.renderVolume = defaultMesh->listOfMeshes["Pyramid"]->VAO;

	candleLight.renderVolume = defaultMesh->listOfMeshes["Sphere"]->VAO;
	candleLight2.renderVolume = defaultMesh->listOfMeshes["Sphere"]->VAO;
	candleLight3.renderVolume = defaultMesh->listOfMeshes["Sphere"]->VAO;
	candleLight4.renderVolume = defaultMesh->listOfMeshes["Sphere"]->VAO;
	candleLight5.renderVolume = defaultMesh->listOfMeshes["Sphere"]->VAO;
	candleLight6.renderVolume = defaultMesh->listOfMeshes["Sphere"]->VAO;
	candleLight7.renderVolume = defaultMesh->listOfMeshes["Sphere"]->VAO;
	candleLight8.renderVolume = defaultMesh->listOfMeshes["Sphere"]->VAO;

	defaultLightPos = glm::vec4(0.0f, 20.0f, 0.0f, 1.0f);

	//INITIALiZE FBOS
	geometryBuffer.Init(windowWidth, windowHeight, 5);
	geometryBuffer.initColorTexture(0, GL_RGBA8);
	geometryBuffer.initColorTexture(1, GL_RGB8);
	geometryBuffer.initColorTexture(2, GL_RGB8);
	geometryBuffer.initColorTexture(3, GL_RGB8);
	geometryBuffer.initColorTexture(4, GL_RGB8);
	geometryBuffer.initDepthTexture();

	finalSceneFBO1.Init(windowWidth, windowHeight, 1);
	finalSceneFBO1.initColorTexture(0, GL_RGB8);
	finalSceneFBO2.Init(windowWidth, windowHeight, 1);
	finalSceneFBO2.initColorTexture(0, GL_RGB8);

	bloomFBO1.Init(windowWidth/2, windowHeight/2, 1);
	bloomFBO1.initColorTexture(0);

	bloomFBO2.Init(windowWidth / 2, windowHeight / 2, 1);
	bloomFBO2.initColorTexture(0);

	motionBlurFBO.Init(windowWidth, windowHeight, 1);
	motionBlurFBO.initColorTexture(0);

	lightingStencil.Init(windowWidth, windowHeight, 1);
	lightingStencil.initColorTexture(0);
	lightingStencil.initDepthTexture();

	//SPRITE ANIMATIONS
	sceneObjects["Warning"]->UVOffsets.push_back(glm::vec2(0.0f, 0.0f));
	sceneObjects["Warning"]->UVOffsets.push_back(glm::vec2(0.25f, 0.0f));
	sceneObjects["Warning"]->UVOffsets.push_back(glm::vec2(0.5f, 0.0f));
	sceneObjects["Warning"]->UVOffsets.push_back(glm::vec2(0.75f, 0.0f));

	sceneObjects["Warning3"]->UVOffsets.push_back(glm::vec2(0.0f, 0.0f));
	sceneObjects["Warning3"]->UVOffsets.push_back(glm::vec2(0.25f, 0.0f));
	sceneObjects["Warning3"]->UVOffsets.push_back(glm::vec2(0.5f, 0.0f));
	sceneObjects["Warning3"]->UVOffsets.push_back(glm::vec2(0.75f, 0.0f));
	sceneObjects["Warning3"]->UVOffsets.push_back(glm::vec2(0.0f, -0.25f));

	sceneObjects["Warning4"]->UVOffsets.push_back(glm::vec2(0.0f, 0.0f));
	sceneObjects["Warning4"]->UVOffsets.push_back(glm::vec2(0.25f, 0.0f));
	sceneObjects["Warning4"]->UVOffsets.push_back(glm::vec2(0.5f, 0.0f));
	sceneObjects["Warning4"]->UVOffsets.push_back(glm::vec2(0.75f, 0.0f));
	sceneObjects["Warning4"]->UVOffsets.push_back(glm::vec2(0.0f, -0.25f));

	sceneObjects["Warning5"]->UVOffsets.push_back(glm::vec2(0.0f, 0.0f));
	sceneObjects["Warning5"]->UVOffsets.push_back(glm::vec2(0.25f, 0.0f));
	sceneObjects["Warning5"]->UVOffsets.push_back(glm::vec2(0.5f, 0.0f));
	sceneObjects["Warning5"]->UVOffsets.push_back(glm::vec2(0.75f, 0.0f));
	sceneObjects["Warning5"]->UVOffsets.push_back(glm::vec2(0.0f, -0.25f));

	sceneObjects["TimerQuad"]->UVOffsets.push_back(zeroPos);
	sceneObjects["TimerQuad"]->UVOffsets.push_back(onePos);
	sceneObjects["TimerQuad"]->UVOffsets.push_back(twoPos);
	sceneObjects["TimerQuad"]->UVOffsets.push_back(threePos);
	sceneObjects["TimerQuad"]->UVOffsets.push_back(fourPos);
	sceneObjects["TimerQuad"]->UVOffsets.push_back(fivePos);
	sceneObjects["TimerQuad"]->UVOffsets.push_back(sixPos);
	sceneObjects["TimerQuad"]->UVOffsets.push_back(sevenPos);
	sceneObjects["TimerQuad"]->UVOffsets.push_back(eightPos);
	sceneObjects["TimerQuad"]->UVOffsets.push_back(ninePos);
	sceneObjects["TimerQuad"]->UVOffsets.push_back(APos);
	sceneObjects["TimerQuad"]->UVOffsets.push_back(BPos);
	sceneObjects["TimerQuad"]->UVOffsets.push_back(CPos);
	sceneObjects["TimerQuad"]->UVOffsets.push_back(DPos);
	sceneObjects["TimerQuad"]->UVOffsets.push_back(EPos);
	sceneObjects["TimerQuad"]->UVOffsets.push_back(FPos);
	sceneObjects["TimerQuad"]->UVOffsets.push_back(GPos);
	sceneObjects["TimerQuad"]->UVOffsets.push_back(HPos);
	sceneObjects["TimerQuad"]->UVOffsets.push_back(IPos);
	sceneObjects["TimerQuad"]->UVOffsets.push_back(JPos);
	sceneObjects["TimerQuad"]->UVOffsets.push_back(KPos);
	sceneObjects["TimerQuad"]->UVOffsets.push_back(LPos);
	sceneObjects["TimerQuad"]->UVOffsets.push_back(MPos);
	sceneObjects["TimerQuad"]->UVOffsets.push_back(NPos);
	sceneObjects["TimerQuad"]->UVOffsets.push_back(OPos);
	sceneObjects["TimerQuad"]->UVOffsets.push_back(PPos);
	sceneObjects["TimerQuad"]->UVOffsets.push_back(QPos);
	sceneObjects["TimerQuad"]->UVOffsets.push_back(RPos);
	sceneObjects["TimerQuad"]->UVOffsets.push_back(SPos);
	sceneObjects["TimerQuad"]->UVOffsets.push_back(TPos);
	sceneObjects["TimerQuad"]->UVOffsets.push_back(UPos);
	sceneObjects["TimerQuad"]->UVOffsets.push_back(VPos);
	sceneObjects["TimerQuad"]->UVOffsets.push_back(WPos);
	sceneObjects["TimerQuad"]->UVOffsets.push_back(XPos);
	sceneObjects["TimerQuad"]->UVOffsets.push_back(YPos);
	sceneObjects["TimerQuad"]->UVOffsets.push_back(ZPos);
	sceneObjects["TimerQuad"]->UVOffsets.push_back(colonPos);

	sceneObjects["HUD"]->UVOffsets.push_back(glm::vec2(0.0f, 0.0f));
	sceneObjects["HUD"]->UVOffsets.push_back(glm::vec2(0.25f, 0.0f));
	sceneObjects["HUD"]->UVOffsets.push_back(glm::vec2(0.5f, 0.0f));
	sceneObjects["HUD"]->UVOffsets.push_back(glm::vec2(0.75f, 0.0f));

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

	Sounds["dash"] = new Sound();
	Sounds["dash"]->load("../assets/sounds/dash_new.wav", true, false);

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

	//INITIALIZE LUTS
	bourbon.loadData("../assets/LUT/Bourbon 64.cube"); //Zeke 39.cube and Clayton 33.cube is grayscale
	clayton.loadData("../assets/LUT/Clayton 33.cube");

	//SET BOUNDING BOXES

	SpotLight.BBBL = glm::vec2(-4.0f, 4.0f);
	SpotLight.BBFR = glm::vec2(4.0f, -4.0f);

	SpotLight2.BBBL = glm::vec2(-4.0f, 4.0f);
	SpotLight2.BBFR = glm::vec2(4.0f, -4.0f);

	QuadLight.BBBL = glm::vec2(-17.5f, 17.5f);
	QuadLight.BBFR = glm::vec2(17.5f, -17.5f);

	TriLight.BBBL = glm::vec2(-11.7f, 11.7f);
	TriLight.BBFR = glm::vec2(11.7f, -11.7f);

	TriLight2.BBBL = glm::vec2(-11.7f, 11.7f);
	TriLight2.BBFR = glm::vec2(11.7f, -11.7f);

	InvisWall.BBBL = glm::vec2(-50.0f, 1.0f);
	InvisWall.BBFR = glm::vec2(50.0f, -50.0f);

	InvisWall2.BBBL = glm::vec2(-50.0f, 50.0f);
	InvisWall2.BBFR = glm::vec2(50.0f, -1.0f);

	InvisWall3.BBBL = glm::vec2(-50.0f, 50.0f);
	InvisWall3.BBFR = glm::vec2(1.0f, -50.0f);

	InvisWall4.BBBL = glm::vec2(-1.0f, 50.0f);
	InvisWall4.BBFR = glm::vec2(50.0f, -50.0f);

	Nyx.BBBL = glm::vec2( 2.0f, -2.0f);
	Nyx.BBFR = glm::vec2(-2.0f,  2.0f);
	
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
	points[17] = glm::vec3(-23.3f, 0.5f, -23.3f);
	points[18] = glm::vec3(0.0f, 0.5f, -23.3f);
	points[19] = glm::vec3(23.3f, 0.5f, -23.3f);
	points[20] = glm::vec3(23.3f, 0.5f, 0.0f);
	points[21] = glm::vec3(23.3f, 0.5f, 23.3f);
	points[22] = glm::vec3(0.0f, 0.5f, 23.3f);
	points[23] = glm::vec3(-23.3f, 0.5f, -23.3f);
	points[24] = glm::vec3(-23.3f, 0.5f, 0.0f);
	points[25] = glm::vec3(0.0f, 0.5f, 0.0f);
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
	numOfCycles = 0;
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

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	gameWindow->check();
	
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

	if(devCommand.GetKeyDown(ENG::KeyCode::Space) || sf::Joystick::isButtonPressed(0, 7)) // Joystick Button Start "Press Start to Play"
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

		if (Player["Nyx"]->getLifeLost())
		{
			Player["Nyx"]->setLifeLost(false);
			dieChannel = Sounds["die"]->play();
			globalT = 0.99f;
		}

		if (globalT >= 1.0f)
		{
			globalT = 0.0f;
			numOfCycles++;

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
			randomTriPos = randomNumber(17, 26);
			randomTriPos2 = randomNumber(17, 26);

			wasWarned1 = false;
			wasWarned2 = false;

			rampValue += 0.00005f;
		}

		globalT += rampValue;

		totalTime += 1 / 60.0f;

		deltaTime = (totalTime - previousTime);

		previousTime = totalTime;

		defaultShader.sendUniform("uTime", totalTime);

		//TEMPORARY ANIMATION FRAME TESTING
		if (totalTime >= timeOfLastAnim + 0.5f)
		{
			timeOfLastAnim = totalTime;
			animFrame++;
		}
		if (animFrame > 3)
			animFrame = 0;

		if (Player["Nyx"]->getIsDead())
			sceneObjects["HUD"]->uUVOffset = sceneObjects["HUD"]->UVOffsets[3];
		else if (Player["Nyx"]->getNumOfLives() < 1)
			sceneObjects["HUD"]->uUVOffset = sceneObjects["HUD"]->UVOffsets[2];
		else if (Player["Nyx"]->getNumOfLives() < 2)
			sceneObjects["HUD"]->uUVOffset = sceneObjects["HUD"]->UVOffsets[1];
		else
			sceneObjects["HUD"]->uUVOffset = sceneObjects["HUD"]->UVOffsets[0];

		if ((totalTime - Player["Nyx"]->timeOfLastDash) < 1.0f)
			sceneObjects["HUD2"]->uUVOffset = sceneObjects["HUD"]->UVOffsets[0];
		else if ((totalTime - Player["Nyx"]->timeOfLastDash) < 2.0f)
			sceneObjects["HUD2"]->uUVOffset = sceneObjects["HUD"]->UVOffsets[1];
		else if ((totalTime - Player["Nyx"]->timeOfLastDash) < 3.0f)
			sceneObjects["HUD2"]->uUVOffset = sceneObjects["HUD"]->UVOffsets[2];
		else
			sceneObjects["HUD2"]->uUVOffset = sceneObjects["HUD"]->UVOffsets[3];

		sceneObjects["SpotLight"]->uEmissiveAdd = glm::vec3(sinf(globalT * 1.0f) * 0.25f, sinf(globalT * 1.0f) * 0.25f, sinf(globalT * 1.0f) * 0.25f);
		sceneObjects["SpotLight2"]->uEmissiveAdd = glm::vec3(sinf(globalT * 1.0f) * 0.25f, sinf(globalT * 1.0f) * 0.25f, sinf(globalT * 1.0f) * 0.25f);
		
		sceneObjects["SpotLight"]->setPosition(lerp(points[randomLERPStart], points[randomLERPEnd], globalT));
		pointLight.position = (sceneObjects["SpotLight"]->getPosition() + glm::vec3(0.0f, -0.25f, 0.0f));

		sceneObjects["SpotLight2"]->setPosition(bezier(points[randomCurveStart], points[randomCurveControl], points[randomCurveEnd], globalT));
		pointLight2.position = (sceneObjects["SpotLight2"]->getPosition() + glm::vec3(0.0f, -0.25f, 0.0f));

		if ((globalT > 0.7f && globalT < 0.98f) && numOfCycles >= 4 && numOfCycles <= 9)
			sceneObjects["QuadLight"]->setPosition(points[randomQuadPos]);
		else
			sceneObjects["QuadLight"]->setPosition(glm::vec3(0.0f, 0.6f, 100.0f));
		
		pointLight3.position = (sceneObjects["QuadLight"]->getPosition() + glm::vec3(0.0f, -0.4f, 0.0f));

		if ((globalT > 0.7f && globalT < 0.98f) && numOfCycles >= 10)
		{
			sceneObjects["TriLight"]->setPosition(points[randomTriPos]);
			sceneObjects["TriLight2"]->setPosition(points[randomTriPos2]);
		}
		else
		{
			sceneObjects["TriLight"]->setPosition(glm::vec3(0.0f, 0.6f, 100.0f));
			sceneObjects["TriLight2"]->setPosition(glm::vec3(0.0f, 0.6f, 100.0f));
		}

		pointLight4.position = (sceneObjects["TriLight"]->getPosition() + glm::vec3(0.0f, -2.4f, 0.0f));
		pointLight5.position = (sceneObjects["TriLight2"]->getPosition() + glm::vec3(0.0f, -2.4f, 0.0f));

		if (!wasWarned1)
			sceneObjects["Warning"]->setPosition(clamp(sceneObjects["SpotLight"]->getPosition(), RoomMin, RoomMax));

		if (!wasWarned2)
			sceneObjects["Warning2"]->setPosition(clamp(sceneObjects["SpotLight2"]->getPosition(), RoomMin, RoomMax));

		if (globalT < 0.7f && numOfCycles >= 4 && numOfCycles <= 9)
			sceneObjects["Warning3"]->setPosition(points[randomQuadPos]);
		else
			sceneObjects["Warning3"]->setPosition(glm::vec3(0.0f, 0.6f, 55.0f));

		if (globalT < 0.7f && numOfCycles >= 10)
		{
			sceneObjects["Warning4"]->setPosition(points[randomTriPos]);
			sceneObjects["Warning5"]->setPosition(points[randomTriPos2]);
		}
		else
		{
			sceneObjects["Warning4"]->setPosition(glm::vec3(0.0f, 0.6f, 55.0f));
			sceneObjects["Warning5"]->setPosition(glm::vec3(0.0f, 0.6f, 55.0f));
		}
			
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

		if (Player["Nyx"]->getIsDead())
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
		if (timeOfDeath == totalTime)
		{
			HUDGObjects.push_back(sceneObjects["GameOverY"]);
			HUDGObjects.push_back(sceneObjects["GameOverY"]);
			HUDGObjects.push_back(sceneObjects["GameOverO"]);
			HUDGObjects.push_back(sceneObjects["GameOverU"]);
			HUDGObjects.push_back(sceneObjects["GameOverD"]);;
			HUDGObjects.push_back(sceneObjects["GameOverI"]);;
			HUDGObjects.push_back(sceneObjects["GameOverE"]);
			HUDGObjects.push_back(sceneObjects["GameOverD2"]);
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

		warningPos.x = sceneObjects["Warning4"]->getPosition().x;
		warningPos.y = sceneObjects["Warning4"]->getPosition().y;
		warningPos.z = sceneObjects["Warning4"]->getPosition().z;
											
		warningVel.x = sceneObjects["Warning4"]->getVelocity().x;
		warningVel.y = sceneObjects["Warning4"]->getVelocity().y;
		warningVel.z = sceneObjects["Warning4"]->getVelocity().z;

		warningPos.x = sceneObjects["Warning5"]->getPosition().x;
		warningPos.y = sceneObjects["Warning5"]->getPosition().y;
		warningPos.z = sceneObjects["Warning5"]->getPosition().z;
											
		warningVel.x = sceneObjects["Warning5"]->getVelocity().x;
		warningVel.y = sceneObjects["Warning5"]->getVelocity().y;
		warningVel.z = sceneObjects["Warning5"]->getVelocity().z;

		Sounds["bgm"]->setPosition(bgmChannel, playerPos, playerVel);
		Sounds["die"]->setPosition(dieChannel, diePos, dieVel);
		Sounds["warn"]->setPosition(warningChannel, warningPos, warningVel);
		Sounds["dash"]->setPosition(dashChannel, playerPos, playerVel);
		
		Sound::Sys.listenerPos = playerPos;
		Sound::Sys.listenerVel = playerVel;
		
		Sound::systemUpdate();

		if (glm::distance(Player["Nyx"]->getPosition(), sceneObjects["Warning3"]->getPosition()) <= 19.0f && !inRadius)
			hasBeenWarned = true;
		if (glm::distance(Player["Nyx"]->getPosition(), sceneObjects["Warning4"]->getPosition()) <= 13.0f && !inRadius)
			hasBeenWarned = true;
		if (glm::distance(Player["Nyx"]->getPosition(), sceneObjects["Warning5"]->getPosition()) <= 13.0f && !inRadius)
			hasBeenWarned = true;

		if (glm::distance(Player["Nyx"]->getPosition(), sceneObjects["Warning3"]->getPosition()) <= 19.0f)
			inRadius = true;
		else if (glm::distance(Player["Nyx"]->getPosition(), sceneObjects["Warning4"]->getPosition()) <= 13.0f)
			inRadius = true;
		else if (glm::distance(Player["Nyx"]->getPosition(), sceneObjects["Warning5"]->getPosition()) <= 13.0f)
			inRadius = true;
		else
			inRadius = false; 
		
		if (hasBeenWarned)
		{
			warningChannel = Sounds["warn"]->play();
			hasBeenWarned = false;
		}

		if(Player["Nyx"]->getDashed())
			dashChannel = Sounds["dash"]->play();

		//Check Window Status
		gameWindow->check();
		
		//Drawing Geometry and Player Character
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		geometryBuffer.Bind();
		GBuffer.bind();

		GBuffer.sendUniformMat4("uView", &view.getMatrix()[0][0], false);
		GBuffer.sendUniformMat4("uProj", &persp[0][0], false);
		GBuffer.sendUniform("LightPosition", defaultLightPos);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

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

		//Drawing Transparent Objects
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		defaultShader.bind();

		sceneObjects["Warning"]->uUVOffset = sceneObjects["Warning"]->UVOffsets[animFrame];
		sceneObjects["Warning2"]->uUVOffset = sceneObjects["Warning"]->UVOffsets[animFrame];
		sceneObjects["Warning3"]->uUVOffset = sceneObjects["Warning3"]->UVOffsets[animFrame];
		sceneObjects["Warning4"]->uUVOffset = sceneObjects["Warning4"]->UVOffsets[animFrame];
		sceneObjects["Warning5"]->uUVOffset = sceneObjects["Warning5"]->UVOffsets[animFrame];

		defaultShader.sendUniformMat4("uView", &view.getMatrix()[0][0], false);
		defaultShader.sendUniformMat4("uProj", &persp[0][0], false);
		defaultShader.sendUniform("LightPosition", defaultLightPos);

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
		geometryBuffer.Unbind();
		
		//Lighting Prepass---------------------------------

		geometryBuffer.BlitDepthToTargetBuffer(&lightingStencil);
		
		lightingStencil.Bind();
		
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);

		geometryBuffer.BindColorAsTexture(GL_TEXTURE0, 0);
		geometryBuffer.BindColorAsTexture(GL_TEXTURE1, 1);
		geometryBuffer.BindColorAsTexture(GL_TEXTURE2, 2);
		geometryBuffer.BindColorAsTexture(GL_TEXTURE3, 3);
		geometryBuffer.BindColorAsTexture(GL_TEXTURE4, 4);
		geometryBuffer.BindDepthAsTexture(GL_TEXTURE5);


		deferredLighting.bind();

		viewInverse = glm::inverse(view.getMatrix());

		deferredLighting.sendUniformMat4("uView", &view.getMatrix()[0][0], false);
		deferredLighting.sendUniformMat4("uProj", &persp[0][0], false);
		deferredLighting.sendUniformMat4("uinverseViewMatrix", &viewInverse[0][0], false);
		deferredLighting.sendUniformMat4("uinversePerspectiveMatrix", &glm::inverse(persp)[0][0], false);

		updatedCamPos = viewInverse[3];
		deferredLighting.sendUniform("uCameraPos", updatedCamPos);

		//glDepthMask(GL_FALSE);

		for (auto itr = lightObjects.begin(), itrEnd = lightObjects.end();
			itr != itrEnd; itr++)
		{
			deferredLighting.sendUniformPointLight("uCurrentLight", (*itr));

			//(*itr)->position = glm::vec3(0.0f, 10.0f, 0.0f);

			(*itr)->render(defaultMesh, &deferredLighting);
		}

		//glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);

		deferredLighting.unBind();
		lightingStencil.Unbind();

		//-------------------------------------------------

		//Lighting Composite

		finalSceneFBO1.Bind();
		lightingComposite.bind();

		viewInverse = glm::inverse(view.getMatrix());

		glBindVertexArray(sceneObjects["Quad3"]->getRenderable());

		geometryBuffer.BindColorAsTexture(GL_TEXTURE0, 0);
		lightingStencil.BindColorAsTexture(GL_TEXTURE1, 0);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindVertexArray(0);

		lightingComposite.unBind();
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
		
		//Looping Blur Passes
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
		
		// Motion Blur
		finalSceneFBO1.Bind();
		motionBlur.bind();
		finalSceneFBO2.BindColorAsTexture(GL_TEXTURE0, 0);
		motionBlurFBO.BindColorAsTexture(GL_TEXTURE1, 0);

		motionBlur.sendUniform("uTex", 0);
		motionBlur.sendUniform("uPrevScene", 1);
		motionBlur.sendUniform("uBlurAmount", clamp(1.0f - (totalTime - Player["Nyx"]->timeOfLastDash), 0.0f, 0.75f));
		
		//timeOfLastDash
		glBindVertexArray(sceneObjects["Quad3"]->getRenderable());
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		motionBlur.unBind();
		finalSceneFBO1.Unbind();

		//Copy Scene for next use
		motionBlurFBO.Bind();
		passThrough.bind();
		finalSceneFBO1.BindColorAsTexture(GL_TEXTURE0, 0);
		passThrough.sendUniform("uTex", 0);

		glBindVertexArray(sceneObjects["Quad3"]->getRenderable());
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		passThrough.unBind();
		motionBlurFBO.Unbind();

		//Drawing HUD
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		UVScrolling.bind();
		finalSceneFBO1.Bind();

		sceneObjects["TimerQuad"]->uUVOffset = sceneObjects["TimerQuad"]->UVOffsets[dinutesPos];
		sceneObjects["TimerQuad2"]->uUVOffset = sceneObjects["TimerQuad"]->UVOffsets[minutesPos];
		//COLON
		sceneObjects["TimerQuad4"]->uUVOffset = sceneObjects["TimerQuad"]->UVOffsets[decondsPos];
		sceneObjects["TimerQuad5"]->uUVOffset = sceneObjects["TimerQuad"]->UVOffsets[secondsPos];

		UVScrolling.sendUniformMat4("uView", &view.getMatrix()[0][0], false);
		UVScrolling.sendUniformMat4("uProj", &orthoPersp[0][0], false);
		UVScrolling.sendUniform("LightPosition", down);

		for (auto itr = HUDGObjects.begin(), itrEnd = HUDGObjects.end();
			itr != itrEnd; itr++)
		{
			(*itr)->update(deltaTime, totalTime);
		}

		for (auto itr = HUDGObjects.begin(), itrEnd = HUDGObjects.end();
			itr != itrEnd; itr++)
		{
			(*itr)->render(defaultMesh, &UVScrolling);
		}

		UVScrolling.unBind();

		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);

		finalSceneFBO1.Unbind();

		finalSceneFBO2.Bind();
		colorCorrect.bind();

		finalSceneFBO1.BindColorAsTexture(GL_TEXTURE0, 0);
		colorCorrect.sendUniform("t", 0.5f);
		bourbon.bind(GL_TEXTURE1);

		if (Player["Nyx"]->getIsDead())
		{
			colorCorrect.sendUniform("t", clamp((totalTime - timeOfDeath) * 0.5f, 0.0f, 1.0f));
			clayton.bind(GL_TEXTURE1);
		}

		glBindVertexArray(sceneObjects["Quad3"]->getRenderable());
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		finalSceneFBO2.Unbind();
		//finalSceneFBO1.Bind();
		//passThrough.bind();
		//
		//lightingStencil.BindDepthAsTexture(GL_TEXTURE0);
		//
		//passThrough.sendUniform("uTex", 0);
		//
		//glBindVertexArray(sceneObjects["Quad3"]->getRenderable());
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		//glBindVertexArray(0);
		//
		//passThrough.unBind();
		//finalSceneFBO1.Unbind();
		
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

	seconds = 0.0f;
	minute = 0.0f;

	//POSITION OBJECTS
	Reset();

	bgmChannel = Sounds["bgm"]->play();

	gObjects.push_back(Player["Nyx"]);
	Player["Nyx"]->setStartPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	Player["Nyx"]->setPosition(Player["Nyx"]->getStartPosition());

	gObjects.push_back(sceneObjects["Room"]);
	sceneObjects["Room"]->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));

	gObjects.push_back(sceneObjects["Candle"]);
	sceneObjects["Candle"]->getTransform()->rotateY(-1.570795f);
	sceneObjects["Candle"]->setPosition(glm::vec3(-35.0f, 4.0f, 17.5f));
	gObjects.push_back(sceneObjects["Candle2"]);
	sceneObjects["Candle2"]->getTransform()->rotateY(-1.570795f);
	sceneObjects["Candle2"]->setPosition(glm::vec3(-35.0f, 4.0f, -17.5f)); 
	gObjects.push_back(sceneObjects["Candle3"]);
	sceneObjects["Candle3"]->getTransform()->rotateY(1.570795f);
	sceneObjects["Candle3"]->setPosition(glm::vec3(35.0f, 4.0f, 17.5f));
	gObjects.push_back(sceneObjects["Candle4"]);
	sceneObjects["Candle4"]->getTransform()->rotateY(1.570795f);
	sceneObjects["Candle4"]->setPosition(glm::vec3(35.0f, 4.0f, -17.5f));
	gObjects.push_back(sceneObjects["Candle5"]);
	//DO NOT ROTATE
	sceneObjects["Candle5"]->setPosition(glm::vec3(17.5f, 4.0f, 35.0f));
	gObjects.push_back(sceneObjects["Candle6"]);
	//DO NOT ROTATE
	sceneObjects["Candle6"]->setPosition(glm::vec3(-17.5f, 4.0f, 35.0f));
	gObjects.push_back(sceneObjects["Candle7"]);
	sceneObjects["Candle7"]->getTransform()->rotateY(3.14159f);
	sceneObjects["Candle7"]->setPosition(glm::vec3(17.5f, 4.0f, -35.0f));
	gObjects.push_back(sceneObjects["Candle8"]);
	sceneObjects["Candle8"]->getTransform()->rotateY(3.14159f);
	sceneObjects["Candle8"]->setPosition(glm::vec3(-17.5f, 4.0f, -35.0f));

	gObjects.push_back(sceneObjects["SpotLight"]);
	sceneObjects["SpotLight"]->setLoss(true);
	sceneObjects["SpotLight"]->setPosition(glm::vec3(0.0f, 0.6f, -55.0f));

	gObjects.push_back(sceneObjects["SpotLight2"]);
	sceneObjects["SpotLight2"]->setLoss(true);
	sceneObjects["SpotLight2"]->setPosition(glm::vec3(0.0f, 0.6f, -55.0f));

	gObjects.push_back(sceneObjects["QuadLight"]);
	sceneObjects["QuadLight"]->setLoss(true);
	sceneObjects["QuadLight"]->setPosition(glm::vec3(0.0f, 0.6f, -55.0f));

	gObjects.push_back(sceneObjects["TriLight"]);
	sceneObjects["TriLight"]->setLoss(true);
	sceneObjects["TriLight"]->setPosition(glm::vec3(0.0f, 0.6f, -55.0f));

	gObjects.push_back(sceneObjects["TriLight2"]);
	sceneObjects["TriLight2"]->setLoss(true);
	sceneObjects["TriLight2"]->setPosition(glm::vec3(0.0f, 0.6f, -55.0f));

	transparentGObjects.push_back(sceneObjects["Warning5"]);
	sceneObjects["Warning5"]->setPosition(glm::vec3(0.0f, 10.0f, -55.0f));
	transparentGObjects.push_back(sceneObjects["Warning4"]);
	sceneObjects["Warning4"]->setPosition(glm::vec3(0.0f, 10.0f, -55.0f));
	transparentGObjects.push_back(sceneObjects["Warning3"]);
	sceneObjects["Warning3"]->setPosition(glm::vec3(0.0f, 10.0f, -55.0f));
	transparentGObjects.push_back(sceneObjects["Warning"]);
	sceneObjects["Warning"]->setPosition(glm::vec3(0.0f, 10.0f, -55.0f));
	transparentGObjects.push_back(sceneObjects["Warning2"]);
	sceneObjects["Warning2"]->setPosition(glm::vec3(0.0f, 10.0f, -55.0f));

	HUDGObjects.push_back(sceneObjects["NumPlate"]);
	sceneObjects["NumPlate"]->setPosition(glm::vec3(0.0f, 0.0f, -4.4f));
	HUDGObjects.push_back(sceneObjects["TimerQuad"]);
	sceneObjects["TimerQuad"]->setPosition(glm::vec3(-1.1f, 0.0f, -4.4f));
	HUDGObjects.push_back(sceneObjects["TimerQuad2"]);
	sceneObjects["TimerQuad2"]->setPosition(glm::vec3(-0.5f, 0.0f, -4.4f));
	HUDGObjects.push_back(sceneObjects["TimerQuad3"]);		
	sceneObjects["TimerQuad3"]->setPosition(glm::vec3(0.0f, 0.0f, -4.4f));
	HUDGObjects.push_back(sceneObjects["TimerQuad4"]);		
	sceneObjects["TimerQuad4"]->setPosition(glm::vec3(0.5f, 0.0f, -4.4f));
	HUDGObjects.push_back(sceneObjects["TimerQuad5"]);		
	sceneObjects["TimerQuad5"]->setPosition(glm::vec3(1.1f, 0.0f, -4.4f));
	HUDGObjects.push_back(sceneObjects["HUD"]);
	sceneObjects["HUD"]->setPosition(glm::vec3(-3.2f, 0.0f, -4.4f));
	HUDGObjects.push_back(sceneObjects["HUD2"]);
	sceneObjects["HUD2"]->setPosition(glm::vec3(3.0f, 0.0f, -4.4f));

	sceneObjects["GameOverY"]->setPosition(glm::vec3(-1.2f, 0.0f, 0.0f));
	sceneObjects["GameOverO"]->setPosition(glm::vec3(-0.8f, 0.0f, 0.0f));
	sceneObjects["GameOverU"]->setPosition(glm::vec3(-0.4f, 0.0f, 0.0f));
	sceneObjects["GameOverD"]->setPosition(glm::vec3(0.2f, 0.0f, 0.0f));
	sceneObjects["GameOverI"]->setPosition(glm::vec3(0.55f, 0.0f, 0.0f));
	sceneObjects["GameOverE"]->setPosition(glm::vec3(0.8f, 0.0f, 0.0f));
	sceneObjects["GameOverD2"]->setPosition(glm::vec3(1.2f, 0.0f, 0.0f));

	sceneObjects["InvisWall"]->setPosition(glm::vec3(0.0f, 0.0f, -35.0f));
	sceneObjects["InvisWall2"]->setPosition(glm::vec3(0.0f, 0.0f, 35.0f));
	sceneObjects["InvisWall3"]->setPosition(glm::vec3(-35.0f, 0.0f, 0.0f));
	sceneObjects["InvisWall4"]->setPosition(glm::vec3(35.0f, 0.0f, 0.0f));

	Player["Nyx"]->uDiffuseAdd = glm::vec3(0.0f, 0.0f, 0.0f);
	Player["Nyx"]->uDiffuseMult = glm::vec3(1.0f, 1.0f, 1.0f);
	Player["Nyx"]->uSpecularAdd = glm::vec3(10.0f, 10.0f, 10.0f);
	Player["Nyx"]->uSpecularMult = glm::vec3(2.0f, 2.0f, 2.0f);
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
	sceneObjects["Warning4"]->uAmbientMult = glm::vec3(4.0f, 4.0f, 4.0f);
	sceneObjects["Warning5"]->uAmbientMult = glm::vec3(4.0f, 4.0f, 4.0f);

	sceneObjects["SpotLight"]->uDiffuseMult = glm::vec3(1.0f, 1.0f, 1.0f);
	sceneObjects["SpotLight2"]->uDiffuseMult = glm::vec3(1.0f, 1.0f, 1.0f);
	sceneObjects["QuadLight"]->uDiffuseMult = glm::vec3(1.0f, 1.0f, 1.0f);
	sceneObjects["TriLight"]->uDiffuseMult = glm::vec3(1.0f, 1.0f, 1.0f);
	sceneObjects["TriLight2"]->uDiffuseMult = glm::vec3(1.0f, 1.0f, 1.0f);

	sceneObjects["TimerQuad"]->uUVOffset = glm::vec2(0.0f, 0.0f);
	sceneObjects["TimerQuad2"]->uUVOffset = glm::vec2(0.0f, 0.0f);
	sceneObjects["TimerQuad3"]->uUVOffset = colonPos;
	sceneObjects["TimerQuad4"]->uUVOffset = glm::vec2(0.0f, 0.0f);
	sceneObjects["TimerQuad5"]->uUVOffset = glm::vec2(0.0f, 0.0f);

	sceneObjects["GameOverY"]->uUVOffset = YPos;
	sceneObjects["GameOverO"]->uUVOffset = OPos;
	sceneObjects["GameOverU"]->uUVOffset = UPos;
	sceneObjects["GameOverD"]->uUVOffset = DPos;
	sceneObjects["GameOverI"]->uUVOffset = IPos;
	sceneObjects["GameOverE"]->uUVOffset = EPos;
	sceneObjects["GameOverD2"]->uUVOffset = DPos;

	sceneObjects["Warning"]->uUVOffset = glm::vec2(0.0f, 0.0f);
	sceneObjects["Warning"]->uUVOffset = glm::vec2(0.0f, 0.0f);
	sceneObjects["Warning"]->uUVOffset = glm::vec2(0.0f, 0.0f);
	sceneObjects["Warning"]->uUVOffset = glm::vec2(0.0f, 0.0f);

	sceneObjects["Warning3"]->uUVOffset = glm::vec2(0.0f, 0.0f);
	sceneObjects["Warning3"]->uUVOffset = glm::vec2(0.0f, 0.0f);
	sceneObjects["Warning3"]->uUVOffset = glm::vec2(0.0f, 0.0f);
	sceneObjects["Warning3"]->uUVOffset = glm::vec2(0.0f, 0.0f);

	sceneObjects["Warning4"]->uUVOffset = glm::vec2(0.0f, 0.0f);
	sceneObjects["Warning4"]->uUVOffset = glm::vec2(0.0f, 0.0f);
	sceneObjects["Warning4"]->uUVOffset = glm::vec2(0.0f, 0.0f);
	sceneObjects["Warning4"]->uUVOffset = glm::vec2(0.0f, 0.0f);

	sceneObjects["Warning5"]->uUVOffset = glm::vec2(0.0f, 0.0f);
	sceneObjects["Warning5"]->uUVOffset = glm::vec2(0.0f, 0.0f);
	sceneObjects["Warning5"]->uUVOffset = glm::vec2(0.0f, 0.0f);
	sceneObjects["Warning5"]->uUVOffset = glm::vec2(0.0f, 0.0f);

	if (!hasLoadedOnce)
	{
		//PUSH BACK COLLIDABLE OBJECTS

		collidables.push_back(sceneObjects["SpotLight"]);
		collidables.push_back(sceneObjects["SpotLight2"]);
		collidables.push_back(sceneObjects["QuadLight"]);
		collidables.push_back(sceneObjects["TriLight"]);
		collidables.push_back(sceneObjects["TriLight2"]);

		collidables.push_back(sceneObjects["InvisWall"]);
		collidables.push_back(sceneObjects["InvisWall2"]);
		collidables.push_back(sceneObjects["InvisWall3"]);
		collidables.push_back(sceneObjects["InvisWall4"]);

	    lightObjects.push_back(&pointLight);
	    lightObjects.push_back(&pointLight2);
	    lightObjects.push_back(&pointLight3);
	    lightObjects.push_back(&pointLight4);
	    lightObjects.push_back(&pointLight5);

		lightObjects.push_back(&candleLight);
		lightObjects.push_back(&candleLight2);
		lightObjects.push_back(&candleLight3);
		lightObjects.push_back(&candleLight4);
		lightObjects.push_back(&candleLight5);
		lightObjects.push_back(&candleLight6);
		lightObjects.push_back(&candleLight7);
		lightObjects.push_back(&candleLight8);
		
		//pointLight.position = sceneObjects["SpotLight"]->getPosition();
		pointLight.color = glm::vec4(1.0f, 0.63f, 0.72f, 1.0f);
		pointLight.meshName = "Cone";

		//pointLight2.position = sceneObjects["SpotLight2"]->getPosition();
		pointLight2.color = glm::vec4(0.48f, 0.41f, 0.93f, 1.0f);
		pointLight2.meshName = "Cone";

		//pointLight3.position = sceneObjects["QuadLight"]->getPosition();
		pointLight3.color = glm::vec4(0.3f, 0.7f, 0.4f, 1.0f);
		pointLight3.aConstant = pointLight3.aConstant * 0.1f;
		pointLight3.aLinear = pointLight3.aLinear * 0.1f;
		pointLight3.aQuadratic = pointLight3.aQuadratic * 0.1f;
		pointLight3.meshName = "Pyramid";

		//pointLight4.position = sceneObjects["TriLight"]->getPosition();
		pointLight4.color = glm::vec4(0.3f, 0.7f, 0.4f, 1.0f);
		pointLight4.aConstant = pointLight4.aConstant * 0.1f;
		pointLight4.aLinear = pointLight4.aLinear * 0.1f;
		pointLight4.aQuadratic = pointLight4.aQuadratic * 0.1f;
		pointLight4.meshName = "Pyramid";

		//pointLight5.position = sceneObjects["TriLight2"]->getPosition();
		pointLight5.color = glm::vec4(0.3f, 0.7f, 0.4f, 1.0f);
		pointLight5.aConstant = pointLight5.aConstant * 0.1f;
		pointLight5.aLinear = pointLight5.aLinear * 0.1f;
		pointLight5.aQuadratic = pointLight5.aQuadratic * 0.1f;
		pointLight5.meshName = "Pyramid";

		candleLight.color = glm::vec4(1.0f, 0.67f, 0.02f, 1.0f);
		candleLight.position = glm::vec3(-32.5f, 10.0f, 17.5f);
		candleLight.meshName = "Sphere";

		candleLight2 = candleLight;
		candleLight2.position = glm::vec3(-32.5f, 10.0f, -17.5f);
		
		candleLight3 = candleLight;
		candleLight3.position = glm::vec3(32.5f, 10.0f, 17.5f);

		candleLight4 = candleLight;
		candleLight4.position = glm::vec3(32.5f, 10.0f, -17.5f);

		candleLight5 = candleLight;
		candleLight5.position = glm::vec3(17.5f, 10.0f, 32.5f);

		candleLight6 = candleLight;
		candleLight6.position = glm::vec3(-17.5f, 10.0f, 32.5f);

		candleLight7 = candleLight;
		candleLight7.position = glm::vec3(17.5f, 10.0f, -32.5f);

		candleLight8 = candleLight;
		candleLight8.position = glm::vec3(-17.5f, 10.0f, -32.5f);

		hasLoadedOnce = true;
	}
}

void GameLevel::gameOver()
{
	removeGameObjects();

	Sounds["bgm"]->channel->stop();
	Sounds["die"]->channel->stop();
	Sounds["warn"]->channel->stop();
	Sounds["dash"]->channel->stop();

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

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	gameWindow->check();

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

	if (devCommand.GetKeyDown(ENG::KeyCode::Space) || sf::Joystick::isButtonPressed(0, 0)) // Joystick button A "Press A to go back to main menu 
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

	if (int(seconds) % 10 < 1)				secondsPos = 0;
	else if (int(seconds) % 10 < 2)			secondsPos = 1;
	else if (int(seconds) % 10 < 3)			secondsPos = 2;
	else if (int(seconds) % 10 < 4)			secondsPos = 3;
	else if (int(seconds) % 10 < 5)			secondsPos = 4;
	else if (int(seconds) % 10 < 6)			secondsPos = 5;
	else if (int(seconds) % 10 < 7)			secondsPos = 6;
	else if (int(seconds) % 10 < 8)			secondsPos = 7;
	else if (int(seconds) % 10 < 9)			secondsPos = 8;
	else if ((int(seconds) % 10) < 10)		secondsPos = 9;
	
	if ((seconds / 10) < 1)					decondsPos = 0;
	else if ((int(seconds) / 10) < 2)		decondsPos = 1;
	else if ((int(seconds) / 10) < 3)		decondsPos = 2;
	else if ((int(seconds) / 10) < 4)		decondsPos = 3;
	else if ((int(seconds) / 10) < 5)		decondsPos = 4;
	else if ((int(seconds) / 10) < 6)		decondsPos = 5;
	else if ((int(seconds) / 10) < 7)		decondsPos = 6;
	else if ((int(seconds) / 10) < 8)		decondsPos = 7;
	else if ((int(seconds) / 10) < 9)		decondsPos = 8;
	else if ((int(seconds) / 10) < 10)		decondsPos = 9;

	if (int(minute) < 1)					minutesPos = 0;
	else if (int(minute) < 2)				minutesPos = 1;
	else if (int(minute) < 3)				minutesPos = 2;
	else if (int(minute) < 4)				minutesPos = 3;
	else if (int(minute) < 5)				minutesPos = 4;
	else if (int(minute) < 6)				minutesPos = 5;
	else if (int(minute) < 7)				minutesPos = 6;
	else if (int(minute) < 8)				minutesPos = 7;
	else if (int(minute) < 9)				minutesPos = 8;
	else if ((int(minute) / 10) < 10)		minutesPos = 9;

	if ((int(minute) / 10) < 1)				dinutesPos = 0;
	else if ((int(minute) / 10) < 2)		dinutesPos = 1;
	else if ((int(minute) / 10) < 3)		dinutesPos = 2;
	else if ((int(minute) / 10) < 4)		dinutesPos = 3;
	else if ((int(minute) / 10) < 5)		dinutesPos = 4;
	else if ((int(minute) / 10) < 6)		dinutesPos = 5;
	else if ((int(minute) / 10) < 7)		dinutesPos = 6;
	else if ((int(minute) / 10) < 8)		dinutesPos = 7;
	else if ((int(minute) / 10) < 9)		dinutesPos = 8;
	else if ((int(minute) / 10) < 10)		dinutesPos = 9;


	//std::cout << minute << " : " << seconds << std::endl;
	return seconds;

}

void removeGameObjects()
{
	gObjects.clear();
	transparentGObjects.clear();
	HUDGObjects.clear();
}