#include "States.h"

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
	
	//LOAD THINGS ONLY NECESSARY FOR LOADING SCREEN
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
	defaultTexture->LoadFromFile("Tutorial", "../assets/textures/tutorial.png");
	static ENG::SceneObject Quad("Quad", defaultMesh->listOfMeshes["Quad"]->VAO, *defaultTexture->listOfTextures["MainMenu"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], geometryBuffer.getLayerNumber());
	sceneObjects["Quad"] = &Quad;
	static ENG::SceneObject Quad2("Quad", defaultMesh->listOfMeshes["Quad"]->VAO, *defaultTexture->listOfTextures["Tutorial"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], geometryBuffer.getLayerNumber());
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

	static ENG::SceneObject ScoreQuad("TimerQuad", defaultMesh->listOfMeshes["TimerQuad"]->VAO, *defaultTexture->listOfTextures["numTexture"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], geometryBuffer.getLayerNumber());
	sceneObjects["ScoreQuad"] = &ScoreQuad;
	static ENG::SceneObject ScoreQuad2("TimerQuad", defaultMesh->listOfMeshes["TimerQuad"]->VAO, *defaultTexture->listOfTextures["numTexture"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], geometryBuffer.getLayerNumber());
	sceneObjects["ScoreQuad2"] = &ScoreQuad2;
	static ENG::SceneObject ScoreQuad3("TimerQuad", defaultMesh->listOfMeshes["TimerQuad"]->VAO, *defaultTexture->listOfTextures["numTexture"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], geometryBuffer.getLayerNumber());
	sceneObjects["ScoreQuad3"] = &ScoreQuad3;
	static ENG::SceneObject ScoreQuad4("TimerQuad", defaultMesh->listOfMeshes["TimerQuad"]->VAO, *defaultTexture->listOfTextures["numTexture"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], geometryBuffer.getLayerNumber());
	sceneObjects["ScoreQuad4"] = &ScoreQuad4;
	static ENG::SceneObject ScoreQuad5("TimerQuad", defaultMesh->listOfMeshes["TimerQuad"]->VAO, *defaultTexture->listOfTextures["numTexture"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], geometryBuffer.getLayerNumber());
	sceneObjects["ScoreQuad5"] = &ScoreQuad5;

	static ENG::SceneObject ScoreQuad6("TimerQuad", defaultMesh->listOfMeshes["TimerQuad"]->VAO, *defaultTexture->listOfTextures["numTexture"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], geometryBuffer.getLayerNumber());
	sceneObjects["ScoreQuad6"] = &ScoreQuad6;
	static ENG::SceneObject ScoreQuad7("TimerQuad", defaultMesh->listOfMeshes["TimerQuad"]->VAO, *defaultTexture->listOfTextures["numTexture"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], geometryBuffer.getLayerNumber());
	sceneObjects["ScoreQuad7"] = &ScoreQuad7;
	static ENG::SceneObject ScoreQuad8("TimerQuad", defaultMesh->listOfMeshes["TimerQuad"]->VAO, *defaultTexture->listOfTextures["numTexture"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], geometryBuffer.getLayerNumber());
	sceneObjects["ScoreQuad8"] = &ScoreQuad8;
	static ENG::SceneObject ScoreQuad9("TimerQuad", defaultMesh->listOfMeshes["TimerQuad"]->VAO, *defaultTexture->listOfTextures["numTexture"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], geometryBuffer.getLayerNumber());
	sceneObjects["ScoreQuad9"] = &ScoreQuad9;
	static ENG::SceneObject ScoreQuad10("TimerQuad", defaultMesh->listOfMeshes["TimerQuad"]->VAO, *defaultTexture->listOfTextures["numTexture"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], geometryBuffer.getLayerNumber());
	sceneObjects["ScoreQuad10"] = &ScoreQuad10;

	static ENG::SceneObject ScoreQuad11("TimerQuad", defaultMesh->listOfMeshes["TimerQuad"]->VAO, *defaultTexture->listOfTextures["numTexture"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], geometryBuffer.getLayerNumber());
	sceneObjects["ScoreQuad11"] = &ScoreQuad11;
	static ENG::SceneObject ScoreQuad12("TimerQuad", defaultMesh->listOfMeshes["TimerQuad"]->VAO, *defaultTexture->listOfTextures["numTexture"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], geometryBuffer.getLayerNumber());
	sceneObjects["ScoreQuad12"] = &ScoreQuad12;
	static ENG::SceneObject ScoreQuad13("TimerQuad", defaultMesh->listOfMeshes["TimerQuad"]->VAO, *defaultTexture->listOfTextures["numTexture"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], geometryBuffer.getLayerNumber());
	sceneObjects["ScoreQuad13"] = &ScoreQuad13;
	static ENG::SceneObject ScoreQuad14("TimerQuad", defaultMesh->listOfMeshes["TimerQuad"]->VAO, *defaultTexture->listOfTextures["numTexture"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], geometryBuffer.getLayerNumber());
	sceneObjects["ScoreQuad14"] = &ScoreQuad14;
	static ENG::SceneObject ScoreQuad15("TimerQuad", defaultMesh->listOfMeshes["TimerQuad"]->VAO, *defaultTexture->listOfTextures["numTexture"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], geometryBuffer.getLayerNumber());
	sceneObjects["ScoreQuad15"] = &ScoreQuad15;

	static ENG::SceneObject ScoreQuad16("TimerQuad", defaultMesh->listOfMeshes["TimerQuad"]->VAO, *defaultTexture->listOfTextures["numTexture"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], geometryBuffer.getLayerNumber());
	sceneObjects["ScoreQuad16"] = &ScoreQuad16;
	static ENG::SceneObject ScoreQuad17("TimerQuad", defaultMesh->listOfMeshes["TimerQuad"]->VAO, *defaultTexture->listOfTextures["numTexture"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], geometryBuffer.getLayerNumber());
	sceneObjects["ScoreQuad17"] = &ScoreQuad17;
	static ENG::SceneObject ScoreQuad18("TimerQuad", defaultMesh->listOfMeshes["TimerQuad"]->VAO, *defaultTexture->listOfTextures["numTexture"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], geometryBuffer.getLayerNumber());
	sceneObjects["ScoreQuad18"] = &ScoreQuad18;
	static ENG::SceneObject ScoreQuad19("TimerQuad", defaultMesh->listOfMeshes["TimerQuad"]->VAO, *defaultTexture->listOfTextures["numTexture"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], geometryBuffer.getLayerNumber());
	sceneObjects["ScoreQuad19"] = &ScoreQuad19;
	static ENG::SceneObject ScoreQuad20("TimerQuad", defaultMesh->listOfMeshes["TimerQuad"]->VAO, *defaultTexture->listOfTextures["numTexture"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], geometryBuffer.getLayerNumber());
	sceneObjects["ScoreQuad20"] = &ScoreQuad20;

	static ENG::SceneObject ScoreQuad21("TimerQuad", defaultMesh->listOfMeshes["TimerQuad"]->VAO, *defaultTexture->listOfTextures["numTexture"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], geometryBuffer.getLayerNumber());
	sceneObjects["ScoreQuad21"] = &ScoreQuad21;
	static ENG::SceneObject ScoreQuad22("TimerQuad", defaultMesh->listOfMeshes["TimerQuad"]->VAO, *defaultTexture->listOfTextures["numTexture"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], geometryBuffer.getLayerNumber());
	sceneObjects["ScoreQuad22"] = &ScoreQuad22;
	static ENG::SceneObject ScoreQuad23("TimerQuad", defaultMesh->listOfMeshes["TimerQuad"]->VAO, *defaultTexture->listOfTextures["numTexture"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], geometryBuffer.getLayerNumber());
	sceneObjects["ScoreQuad23"] = &ScoreQuad23;
	static ENG::SceneObject ScoreQuad24("TimerQuad", defaultMesh->listOfMeshes["TimerQuad"]->VAO, *defaultTexture->listOfTextures["numTexture"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], geometryBuffer.getLayerNumber());
	sceneObjects["ScoreQuad24"] = &ScoreQuad24;
	static ENG::SceneObject ScoreQuad25("TimerQuad", defaultMesh->listOfMeshes["TimerQuad"]->VAO, *defaultTexture->listOfTextures["numTexture"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], geometryBuffer.getLayerNumber());
	sceneObjects["ScoreQuad25"] = &ScoreQuad25;

	static ENG::SceneObject ScoreQuad26("TimerQuad", defaultMesh->listOfMeshes["TimerQuad"]->VAO, *defaultTexture->listOfTextures["numTexture"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], geometryBuffer.getLayerNumber());
	sceneObjects["ScoreQuad26"] = &ScoreQuad26;
	static ENG::SceneObject ScoreQuad27("TimerQuad", defaultMesh->listOfMeshes["TimerQuad"]->VAO, *defaultTexture->listOfTextures["numTexture"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], geometryBuffer.getLayerNumber());
	sceneObjects["ScoreQuad27"] = &ScoreQuad27;
	static ENG::SceneObject ScoreQuad28("TimerQuad", defaultMesh->listOfMeshes["TimerQuad"]->VAO, *defaultTexture->listOfTextures["numTexture"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], geometryBuffer.getLayerNumber());
	sceneObjects["ScoreQuad28"] = &ScoreQuad28;
	static ENG::SceneObject ScoreQuad29("TimerQuad", defaultMesh->listOfMeshes["TimerQuad"]->VAO, *defaultTexture->listOfTextures["numTexture"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], geometryBuffer.getLayerNumber());
	sceneObjects["ScoreQuad29"] = &ScoreQuad29;
	static ENG::SceneObject ScoreQuad30("TimerQuad", defaultMesh->listOfMeshes["TimerQuad"]->VAO, *defaultTexture->listOfTextures["numTexture"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], geometryBuffer.getLayerNumber());
	sceneObjects["ScoreQuad30"] = &ScoreQuad30;

	static ENG::SceneObject ScoreH("TimerQuad", defaultMesh->listOfMeshes["TimerQuad"]->VAO, *defaultTexture->listOfTextures["numTexture"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], geometryBuffer.getLayerNumber());
	sceneObjects["ScoreH"] = &ScoreH;
	static ENG::SceneObject ScoreI("TimerQuad", defaultMesh->listOfMeshes["TimerQuad"]->VAO, *defaultTexture->listOfTextures["numTexture"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], geometryBuffer.getLayerNumber());
	sceneObjects["ScoreI"] = &ScoreI;
	static ENG::SceneObject ScoreG("TimerQuad", defaultMesh->listOfMeshes["TimerQuad"]->VAO, *defaultTexture->listOfTextures["numTexture"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], geometryBuffer.getLayerNumber());
	sceneObjects["ScoreG"] = &ScoreG;
	static ENG::SceneObject ScoreH2("TimerQuad", defaultMesh->listOfMeshes["TimerQuad"]->VAO, *defaultTexture->listOfTextures["numTexture"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], geometryBuffer.getLayerNumber());
	sceneObjects["ScoreH2"] = &ScoreH2;
	static ENG::SceneObject ScoreS("TimerQuad", defaultMesh->listOfMeshes["TimerQuad"]->VAO, *defaultTexture->listOfTextures["numTexture"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], geometryBuffer.getLayerNumber());
	sceneObjects["ScoreS"] = &ScoreS;
	static ENG::SceneObject ScoreC("TimerQuad", defaultMesh->listOfMeshes["TimerQuad"]->VAO, *defaultTexture->listOfTextures["numTexture"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], geometryBuffer.getLayerNumber());
	sceneObjects["ScoreC"] = &ScoreC;
	static ENG::SceneObject ScoreO("TimerQuad", defaultMesh->listOfMeshes["TimerQuad"]->VAO, *defaultTexture->listOfTextures["numTexture"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], geometryBuffer.getLayerNumber());
	sceneObjects["ScoreO"] = &ScoreO;
	static ENG::SceneObject ScoreR("TimerQuad", defaultMesh->listOfMeshes["TimerQuad"]->VAO, *defaultTexture->listOfTextures["numTexture"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], geometryBuffer.getLayerNumber());
	sceneObjects["ScoreR"] = &ScoreR;
	static ENG::SceneObject ScoreE("TimerQuad", defaultMesh->listOfMeshes["TimerQuad"]->VAO, *defaultTexture->listOfTextures["numTexture"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], geometryBuffer.getLayerNumber());
	sceneObjects["ScoreE"] = &ScoreE;
	static ENG::SceneObject ScoreS2("TimerQuad", defaultMesh->listOfMeshes["TimerQuad"]->VAO, *defaultTexture->listOfTextures["numTexture"], *defaultTexture->listOfTextures["Normal"], *defaultTexture->listOfTextures["Specular"], *defaultTexture->listOfTextures["Emissive"], geometryBuffer.getLayerNumber());
	sceneObjects["ScoreS2"] = &ScoreS2;

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
	defaultMesh->LoadFromFile("Plane", "../assets/objects/light_volumes/plane.obj");

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

	roomLight.renderVolume = defaultMesh->listOfMeshes["Plane"]->VAO;

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

	Sounds["dead"] = new Sound();
	Sounds["dead"]->load("../assets/sounds/game_over.wav", true, false);

	Sounds["candle"] = new Sound();
	Sounds["candle"]->load("../assets/sounds/candle_lit.wav", true, false);

	mainMenuChannel = NULL;
	bgmChannel = NULL;
	dieChannel = NULL;
	warningChannel = NULL;
	pauseMenuChannel = NULL;
	dashChannel = NULL;
	deadChannel = NULL;
	candleChannel = NULL;

	playerPos;
	playerVel;
	diePos;
	dieVel;
	warningPos;
	warningVel;
	candlePos;

	stillSound.x = 0.0f;
	stillSound.y = 0.0f;
	stillSound.z = 0.0f;

	pitchShift = NULL;
	lowPass = NULL;
	echo = NULL;

	//INITIALIZE LUTS
	bourbon.loadData("../assets/LUT/IWLTBAP Aspen - Standard.cube"); //Zeke 39.cube and Clayton 33.cube is grayscale
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

	Candle.BBBL = glm::vec2(-4.0f, 4.0f);
	Candle.BBFR = glm::vec2(4.0f, -4.0f);
	
	Candle2.BBBL = glm::vec2(-4.0f, 4.0f);
	Candle2.BBFR = glm::vec2(4.0f, -4.0f);

	Candle3.BBBL = glm::vec2(-4.0f, 4.0f);
	Candle3.BBFR = glm::vec2(4.0f, -4.0f);

	Candle4.BBBL = glm::vec2(-4.0f, 4.0f);
	Candle4.BBFR = glm::vec2(4.0f, -4.0f);

	Candle5.BBBL = glm::vec2(-4.0f, 4.0f);
	Candle5.BBFR = glm::vec2(4.0f, -4.0f);

	Candle6.BBBL = glm::vec2(-4.0f, 4.0f);
	Candle6.BBFR = glm::vec2(4.0f, -4.0f);

	Candle7.BBBL = glm::vec2(-4.0f, 4.0f);
	Candle7.BBFR = glm::vec2(4.0f, -4.0f);

	Candle8.BBBL = glm::vec2(-4.0f, 4.0f);
	Candle8.BBFR = glm::vec2(4.0f, -4.0f);

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
	sessionTime = 0.0f;
	timeOfDeath = -1.0f;
	numOfCycles = 0;
	deathTimer = false;
	scoreIsUp = false;
	candleOneLit = false;
	candleTwoLit = false;
	candleThreeLit = false;
	candleFourLit = false;
	candleFiveLit = false;
	candleSixLit = false;
	candleSevenLit = false;
	candleEightLit = false;
	animFrame = 0;
	timeOfLastAnim = 0.0f;
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
		m_parent->GetGameState("Tutorial")->SetPaused(false);
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
		Tutorial(hasBeenInitialized);
	}
}


//FIRST LEVEL MAIN LOOP
void GameLevel::Update()
{
	timerFunc(deltaTime);

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
		sessionTime += 1 / 60.0f;

		deltaTime = (totalTime - previousTime);

		previousTime = totalTime;

		defaultShader.sendUniform("uTime", sessionTime);

		//TEMPORARY ANIMATION FRAME TESTING
		if (sessionTime >= timeOfLastAnim + 0.5f)
		{
			timeOfLastAnim = sessionTime;
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

		if ((globalT > 0.7f && globalT < 0.98f) && numOfCycles > 3 && numOfCycles < 25)
			sceneObjects["QuadLight"]->setPosition(points[randomQuadPos]);
		else
			sceneObjects["QuadLight"]->setPosition(glm::vec3(0.0f, 0.6f, 100.0f));
		
		pointLight3.position = (sceneObjects["QuadLight"]->getPosition() + glm::vec3(0.0f, -0.4f, 0.0f));

		if ((globalT > 0.7f && globalT < 0.98f) && numOfCycles > 24)
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

		if (sessionTime > candleLightTime && !candleEightLit)
		{
			candlePos.x = sceneObjects["Candle8"]->getPosition().x;
			candlePos.y = sceneObjects["Candle8"]->getPosition().y;
			candlePos.z = sceneObjects["Candle8"]->getPosition().z;
			Sounds["candle"]->setPosition(candleChannel, candlePos, stillSound);
			candleChannel = Sounds["candle"]->play();
			collidables.push_back(sceneObjects["Candle8"]);
			lightObjects.push_back(&candleLight8);
			candleEightLit = true;
		}
		if (sessionTime > candleLightTime + 0.2f && !candleTwoLit)
		{
			candlePos.x = sceneObjects["Candle2"]->getPosition().x;
			candlePos.y = sceneObjects["Candle2"]->getPosition().y;
			candlePos.z = sceneObjects["Candle2"]->getPosition().z;
			Sounds["candle"]->setPosition(candleChannel, candlePos, stillSound);
			candleChannel = Sounds["candle"]->play();
			collidables.push_back(sceneObjects["Candle2"]);
			lightObjects.push_back(&candleLight2);
			candleTwoLit = true;
		}
		if (sessionTime > candleLightTime + 0.4f && !candleOneLit)
		{
			candlePos.x = sceneObjects["Candle"]->getPosition().x;
			candlePos.y = sceneObjects["Candle"]->getPosition().y;
			candlePos.z = sceneObjects["Candle"]->getPosition().z;
			Sounds["candle"]->setPosition(candleChannel, candlePos, stillSound);
			candleChannel = Sounds["candle"]->play();
			collidables.push_back(sceneObjects["Candle"]);
			lightObjects.push_back(&candleLight);
			candleOneLit = true;
		}
		if (sessionTime > candleLightTime + 0.6f && !candleSixLit)
		{
			candlePos.x = sceneObjects["Candle6"]->getPosition().x;
			candlePos.y = sceneObjects["Candle6"]->getPosition().y;
			candlePos.z = sceneObjects["Candle6"]->getPosition().z;
			Sounds["candle"]->setPosition(candleChannel, candlePos, stillSound);
			candleChannel = Sounds["candle"]->play();
			collidables.push_back(sceneObjects["Candle6"]);
			lightObjects.push_back(&candleLight6);
			candleSixLit = true;
		}
		if (sessionTime > candleLightTime + 0.8f && !candleFiveLit)
		{
			candlePos.x = sceneObjects["Candle5"]->getPosition().x;
			candlePos.y = sceneObjects["Candle5"]->getPosition().y;
			candlePos.z = sceneObjects["Candle5"]->getPosition().z;
			Sounds["candle"]->setPosition(candleChannel, candlePos, stillSound);
			candleChannel = Sounds["candle"]->play();
			collidables.push_back(sceneObjects["Candle5"]);
			lightObjects.push_back(&candleLight5);
			candleFiveLit = true;
		}
		if (sessionTime > candleLightTime + 1.0f && !candleThreeLit)
		{
			candlePos.x = sceneObjects["Candle3"]->getPosition().x;
			candlePos.y = sceneObjects["Candle3"]->getPosition().y;
			candlePos.z = sceneObjects["Candle3"]->getPosition().z;
			Sounds["candle"]->setPosition(candleChannel, candlePos, stillSound);
			candleChannel = Sounds["candle"]->play();
			collidables.push_back(sceneObjects["Candle3"]);
			lightObjects.push_back(&candleLight3);
			candleThreeLit = true;
		}
		if (sessionTime > candleLightTime + 1.2f && !candleFourLit)
		{
			candlePos.x = sceneObjects["Candle4"]->getPosition().x;
			candlePos.y = sceneObjects["Candle4"]->getPosition().y;
			candlePos.z = sceneObjects["Candle4"]->getPosition().z;
			Sounds["candle"]->setPosition(candleChannel, candlePos, stillSound);
			candleChannel = Sounds["candle"]->play();
			collidables.push_back(sceneObjects["Candle4"]);
			lightObjects.push_back(&candleLight4);
			candleFourLit = true;
		}
		if (sessionTime > candleLightTime + 1.4f && !candleSevenLit)
		{
			candlePos.x = sceneObjects["Candle7"]->getPosition().x;
			candlePos.y = sceneObjects["Candle7"]->getPosition().y;
			candlePos.z = sceneObjects["Candle7"]->getPosition().z;
			Sounds["candle"]->setPosition(candleChannel, candlePos, stillSound);
			candleChannel = Sounds["candle"]->play();
			collidables.push_back(sceneObjects["Candle7"]);
			lightObjects.push_back(&candleLight7);
			candleSevenLit = true;
		}
		
		if (devCommand.GetKeyDown(ENG::KeyCode::Add))
		{
			candleLightTime = (sessionTime + 2.0f);
		}

		if (!wasWarned1)
			sceneObjects["Warning"]->setPosition(clamp(sceneObjects["SpotLight"]->getPosition(), RoomMin, RoomMax));

		if (!wasWarned2)
			sceneObjects["Warning2"]->setPosition(clamp(sceneObjects["SpotLight2"]->getPosition(), RoomMin, RoomMax));

		if (globalT < 0.7f && numOfCycles > 3 && numOfCycles < 25)
			sceneObjects["Warning3"]->setPosition(points[randomQuadPos]);
		else
			sceneObjects["Warning3"]->setPosition(glm::vec3(0.0f, 0.6f, 55.0f));

		if (globalT < 0.7f && numOfCycles > 24)
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
				timeOfDeath = sessionTime;
				score.insertScore("seconds", timeOfDeath);
				deathTimer = true;
			}
			if ((deathTimer == true) && (sessionTime > (timeOfDeath + 6.0f)))
			{
				GameLevel::gameOver();
				m_parent->GetGameState("MainMenu")->SetPaused(false);
			}
		}
		if (timeOfDeath == sessionTime)
		{
			deadChannel = Sounds["dead"]->play();

			HUDGObjects.addNode(sceneObjects["GameOverY"], 3);
			HUDGObjects.addNode(sceneObjects["GameOverO"], 3);
			HUDGObjects.addNode(sceneObjects["GameOverU"], 3);
			HUDGObjects.addNode(sceneObjects["GameOverD"], 3);
			HUDGObjects.addNode(sceneObjects["GameOverI"], 3);
			HUDGObjects.addNode(sceneObjects["GameOverE"], 3);
			HUDGObjects.addNode(sceneObjects["GameOverD2"], 3);

			interpretScore();
		}
		if ((deathTimer == true) && (sessionTime - timeOfDeath) > 3.0f && !scoreIsUp)
		{
			for (int i = 0; i < 7; i++)
			{
				HUDGObjects.removeMaxNode();
			}

			HUDGObjects.addNode(sceneObjects["ScoreQuad"], 4);
			HUDGObjects.addNode(sceneObjects["ScoreQuad2"], 4);
			HUDGObjects.addNode(sceneObjects["ScoreQuad3"], 4);
			HUDGObjects.addNode(sceneObjects["ScoreQuad4"], 4);
			HUDGObjects.addNode(sceneObjects["ScoreQuad5"], 4);

			HUDGObjects.addNode(sceneObjects["ScoreQuad6"], 4);
			HUDGObjects.addNode(sceneObjects["ScoreQuad7"], 4);
			HUDGObjects.addNode(sceneObjects["ScoreQuad8"], 4);
			HUDGObjects.addNode(sceneObjects["ScoreQuad9"], 4);
			HUDGObjects.addNode(sceneObjects["ScoreQuad10"], 4);

			HUDGObjects.addNode(sceneObjects["ScoreQuad11"], 4);
			HUDGObjects.addNode(sceneObjects["ScoreQuad12"], 4);
			HUDGObjects.addNode(sceneObjects["ScoreQuad13"], 4);
			HUDGObjects.addNode(sceneObjects["ScoreQuad14"], 4);
			HUDGObjects.addNode(sceneObjects["ScoreQuad15"], 4);

			HUDGObjects.addNode(sceneObjects["ScoreQuad16"], 4);
			HUDGObjects.addNode(sceneObjects["ScoreQuad17"], 4);
			HUDGObjects.addNode(sceneObjects["ScoreQuad18"], 4);
			HUDGObjects.addNode(sceneObjects["ScoreQuad19"], 4);
			HUDGObjects.addNode(sceneObjects["ScoreQuad20"], 4);

			HUDGObjects.addNode(sceneObjects["ScoreQuad21"], 4);
			HUDGObjects.addNode(sceneObjects["ScoreQuad22"], 4);
			HUDGObjects.addNode(sceneObjects["ScoreQuad23"], 4);
			HUDGObjects.addNode(sceneObjects["ScoreQuad24"], 4);
			HUDGObjects.addNode(sceneObjects["ScoreQuad25"], 4);

			HUDGObjects.addNode(sceneObjects["ScoreQuad26"], 4);
			HUDGObjects.addNode(sceneObjects["ScoreQuad27"], 4);
			HUDGObjects.addNode(sceneObjects["ScoreQuad28"], 4);
			HUDGObjects.addNode(sceneObjects["ScoreQuad29"], 4);
			HUDGObjects.addNode(sceneObjects["ScoreQuad30"], 4);

			HUDGObjects.addNode(sceneObjects["ScoreH"], 4);
			HUDGObjects.addNode(sceneObjects["ScoreI"], 4);
			HUDGObjects.addNode(sceneObjects["ScoreG"], 4);
			HUDGObjects.addNode(sceneObjects["ScoreH2"], 4);
			HUDGObjects.addNode(sceneObjects["ScoreS"], 4);
			HUDGObjects.addNode(sceneObjects["ScoreC"], 4);
			HUDGObjects.addNode(sceneObjects["ScoreO"], 4);
			HUDGObjects.addNode(sceneObjects["ScoreR"], 4);
			HUDGObjects.addNode(sceneObjects["ScoreE"], 4);
			HUDGObjects.addNode(sceneObjects["ScoreS2"], 4);

			scoreIsUp = true;
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
		Sounds["dead"]->setPosition(deadChannel, diePos, dieVel);
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

		//Lighting Prepass-Deferred Lighting

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

		glDepthMask(GL_FALSE);

		for (auto itr = lightObjects.begin(), itrEnd = lightObjects.end();
			itr != itrEnd; itr++)
		{
			deferredLighting.sendUniformPointLight("uCurrentLight", (*itr));

			(*itr)->render(defaultMesh, &deferredLighting);
		}

		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);

		deferredLighting.unBind();
		lightingStencil.Unbind();

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

		HUDGObjects.drawList(defaultMesh, &UVScrolling);

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
			colorCorrect.sendUniform("t", clamp((sessionTime - timeOfDeath) * 0.5f, 0.0f, 1.0f));
			clayton.bind(GL_TEXTURE1);
		}

		glBindVertexArray(sceneObjects["Quad3"]->getRenderable());
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		finalSceneFBO2.Unbind();
		
		//DEBUG PASS (FOR VERIFYING OUPUT OF SPECIFIC FBOS)
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
	collidables.clear();
	lightObjects.clear();
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
	sceneObjects["Candle"]->setLoss(true);
	gObjects.push_back(sceneObjects["Candle2"]);
	sceneObjects["Candle2"]->getTransform()->rotateY(-1.570795f);
	sceneObjects["Candle2"]->setPosition(glm::vec3(-35.0f, 4.0f, -17.5f)); 
	sceneObjects["Candle2"]->setLoss(true);
	gObjects.push_back(sceneObjects["Candle3"]);
	sceneObjects["Candle3"]->getTransform()->rotateY(1.570795f);
	sceneObjects["Candle3"]->setPosition(glm::vec3(35.0f, 4.0f, 17.5f));
	sceneObjects["Candle3"]->setLoss(true);
	gObjects.push_back(sceneObjects["Candle4"]);
	sceneObjects["Candle4"]->getTransform()->rotateY(1.570795f);
	sceneObjects["Candle4"]->setPosition(glm::vec3(35.0f, 4.0f, -17.5f));
	sceneObjects["Candle4"]->setLoss(true);
	gObjects.push_back(sceneObjects["Candle5"]);
	//DO NOT ROTATE
	sceneObjects["Candle5"]->setPosition(glm::vec3(17.5f, 4.0f, 35.0f));
	sceneObjects["Candle5"]->setLoss(true);
	gObjects.push_back(sceneObjects["Candle6"]);
	//DO NOT ROTATE
	sceneObjects["Candle6"]->setPosition(glm::vec3(-17.5f, 4.0f, 35.0f));
	sceneObjects["Candle6"]->setLoss(true);
	gObjects.push_back(sceneObjects["Candle7"]);
	sceneObjects["Candle7"]->getTransform()->rotateY(3.14159f);
	sceneObjects["Candle7"]->setPosition(glm::vec3(17.5f, 4.0f, -35.0f));
	sceneObjects["Candle7"]->setLoss(true);
	gObjects.push_back(sceneObjects["Candle8"]);
	sceneObjects["Candle8"]->getTransform()->rotateY(3.14159f);
	sceneObjects["Candle8"]->setPosition(glm::vec3(-17.5f, 4.0f, -35.0f));
	sceneObjects["Candle8"]->setLoss(true);

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

	HUDGObjects.addNode(sceneObjects["NumPlate"], 0);
	sceneObjects["NumPlate"]->setPosition(glm::vec3(0.0f, 0.0f, -4.4f));
	HUDGObjects.addNode(sceneObjects["TimerQuad"], 1);
	sceneObjects["TimerQuad"]->setPosition(glm::vec3(-1.1f, 0.0f, -4.4f));
	HUDGObjects.addNode(sceneObjects["TimerQuad2"], 1);
	sceneObjects["TimerQuad2"]->setPosition(glm::vec3(-0.5f, 0.0f, -4.4f));
	HUDGObjects.addNode(sceneObjects["TimerQuad3"], 1);		
	sceneObjects["TimerQuad3"]->setPosition(glm::vec3(0.0f, 0.0f, -4.4f));
	HUDGObjects.addNode(sceneObjects["TimerQuad4"], 1);		
	sceneObjects["TimerQuad4"]->setPosition(glm::vec3(0.5f, 0.0f, -4.4f));
	HUDGObjects.addNode(sceneObjects["TimerQuad5"], 1);		
	sceneObjects["TimerQuad5"]->setPosition(glm::vec3(1.1f, 0.0f, -4.4f));
	HUDGObjects.addNode(sceneObjects["HUD"], 2);
	sceneObjects["HUD"]->setPosition(glm::vec3(-3.2f, 0.0f, -4.4f));
	HUDGObjects.addNode(sceneObjects["HUD2"], 2);
	sceneObjects["HUD2"]->setPosition(glm::vec3(3.0f, 0.0f, -4.4f));

	sceneObjects["GameOverY"]->setPosition(glm::vec3(-1.2f, 0.0f, 0.0f));
	sceneObjects["GameOverO"]->setPosition(glm::vec3(-0.8f, 0.0f, 0.0f));
	sceneObjects["GameOverU"]->setPosition(glm::vec3(-0.4f, 0.0f, 0.0f));
	sceneObjects["GameOverD"]->setPosition(glm::vec3(0.2f, 0.0f, 0.0f));
	sceneObjects["GameOverI"]->setPosition(glm::vec3(0.55f, 0.0f, 0.0f));
	sceneObjects["GameOverE"]->setPosition(glm::vec3(0.8f, 0.0f, 0.0f));
	sceneObjects["GameOverD2"]->setPosition(glm::vec3(1.2f, 0.0f, 0.0f));

	// First Place Score Line
	sceneObjects["ScoreQuad"]->setPosition(glm::vec3(-0.7f, 0.0f, -2.0f));
	sceneObjects["ScoreQuad2"]->setPosition(glm::vec3(-0.1f, 0.0f, -2.0f));
	sceneObjects["ScoreQuad3"]->setPosition(glm::vec3(0.4f, 0.0f, -2.0f));
	sceneObjects["ScoreQuad4"]->setPosition(glm::vec3(0.9f, 0.0f, -2.0f));
	sceneObjects["ScoreQuad5"]->setPosition(glm::vec3(1.5f, 0.0f, -2.0f));
	// Scond Place Score Line
	sceneObjects["ScoreQuad6"]->setPosition(glm::vec3(-0.7f, 0.0f, -1.0f));
	sceneObjects["ScoreQuad7"]->setPosition(glm::vec3(-0.1f, 0.0f, -1.0f));
	sceneObjects["ScoreQuad8"]->setPosition(glm::vec3(0.4f, 0.0f, -1.0f));
	sceneObjects["ScoreQuad9"]->setPosition(glm::vec3(0.9f, 0.0f, -1.0f));
	sceneObjects["ScoreQuad10"]->setPosition(glm::vec3(1.5f, 0.0f, -1.0f));
	// Third Place Score Line
	sceneObjects["ScoreQuad11"]->setPosition(glm::vec3(-0.7f, 0.0f, -0.0f));
	sceneObjects["ScoreQuad12"]->setPosition(glm::vec3(-0.1f, 0.0f, -0.0f));
	sceneObjects["ScoreQuad13"]->setPosition(glm::vec3(0.4f, 0.0f, -0.0f));
	sceneObjects["ScoreQuad14"]->setPosition(glm::vec3(0.9f, 0.0f, -0.0f));
	sceneObjects["ScoreQuad15"]->setPosition(glm::vec3(1.5f, 0.0f, -0.0f));
	// Fourth Place Score Line
	sceneObjects["ScoreQuad16"]->setPosition(glm::vec3(-0.7f, 0.0f, 1.0f));
	sceneObjects["ScoreQuad17"]->setPosition(glm::vec3(-0.1f, 0.0f, 1.0f));
	sceneObjects["ScoreQuad18"]->setPosition(glm::vec3(0.4f, 0.0f, 1.0f));
	sceneObjects["ScoreQuad19"]->setPosition(glm::vec3(0.9f, 0.0f, 1.0f));
	sceneObjects["ScoreQuad20"]->setPosition(glm::vec3(1.5f, 0.0f, 1.0f));
	// Fifth Place Score Line
	sceneObjects["ScoreQuad21"]->setPosition(glm::vec3(-0.7f, 0.0f, 2.0f));
	sceneObjects["ScoreQuad22"]->setPosition(glm::vec3(-0.1f, 0.0f, 2.0f));
	sceneObjects["ScoreQuad23"]->setPosition(glm::vec3(0.4f, 0.0f, 2.0f));
	sceneObjects["ScoreQuad24"]->setPosition(glm::vec3(0.9f, 0.0f, 2.0f));
	sceneObjects["ScoreQuad25"]->setPosition(glm::vec3(1.5f, 0.0f, 2.0f));
	// Position Of Place Statement
	sceneObjects["ScoreQuad26"]->setPosition(glm::vec3(-1.7f, 0.0f, -2.0f));
	sceneObjects["ScoreQuad27"]->setPosition(glm::vec3(-1.7f, 0.0f, -1.0f));
	sceneObjects["ScoreQuad28"]->setPosition(glm::vec3(-1.7f, 0.0f, 0.0f));
	sceneObjects["ScoreQuad29"]->setPosition(glm::vec3(-1.7f, 0.0f, 1.0f));
	sceneObjects["ScoreQuad30"]->setPosition(glm::vec3(-1.7f, 0.0f, 2.0f));
	// The Word Scores
	sceneObjects["ScoreH"]->setPosition(glm::vec3(-1.9f, 0.0f, -3.0f));
	sceneObjects["ScoreI"]->setPosition(glm::vec3(-1.5f, 0.0f, -3.0f));
	sceneObjects["ScoreG"]->setPosition(glm::vec3(-1.1f, 0.0f, -3.0f));
	sceneObjects["ScoreH2"]->setPosition(glm::vec3(-0.7f, 0.0f, -3.0f));
	sceneObjects["ScoreS"]->setPosition(glm::vec3(-0.1f, 0.0f, -3.0f));
	sceneObjects["ScoreC"]->setPosition(glm::vec3(0.3f, 0.0f, -3.0f));
	sceneObjects["ScoreO"]->setPosition(glm::vec3(0.7f, 0.0f, -3.0f));
	sceneObjects["ScoreR"]->setPosition(glm::vec3(1.1f, 0.0f, -3.0f));
	sceneObjects["ScoreE"]->setPosition(glm::vec3(1.5f, 0.0f, -3.0f));
	sceneObjects["ScoreS2"]->setPosition(glm::vec3(1.9f, 0.0f, -3.0f));

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

	sceneObjects["ScoreQuad"]->uUVOffset = glm::vec2(0.0f, 0.0f);
	sceneObjects["ScoreQuad2"]->uUVOffset = glm::vec2(0.0f, 0.0f);
	sceneObjects["ScoreQuad3"]->uUVOffset = colonPos;
	sceneObjects["ScoreQuad4"]->uUVOffset = glm::vec2(0.0f, 0.0f);
	sceneObjects["ScoreQuad5"]->uUVOffset = glm::vec2(0.0f, 0.0f);

	sceneObjects["ScoreQuad6"]->uUVOffset = glm::vec2(0.0f, 0.0f);
	sceneObjects["ScoreQuad7"]->uUVOffset = glm::vec2(0.0f, 0.0f);
	sceneObjects["ScoreQuad8"]->uUVOffset = colonPos;
	sceneObjects["ScoreQuad9"]->uUVOffset = glm::vec2(0.0f, 0.0f);
	sceneObjects["ScoreQuad10"]->uUVOffset = glm::vec2(0.0f, 0.0f);

	sceneObjects["ScoreQuad11"]->uUVOffset = glm::vec2(0.0f, 0.0f);
	sceneObjects["ScoreQuad12"]->uUVOffset = glm::vec2(0.0f, 0.0f);
	sceneObjects["ScoreQuad13"]->uUVOffset = colonPos;
	sceneObjects["ScoreQuad14"]->uUVOffset = glm::vec2(0.0f, 0.0f);
	sceneObjects["ScoreQuad15"]->uUVOffset = glm::vec2(0.0f, 0.0f);

	sceneObjects["ScoreQuad16"]->uUVOffset = glm::vec2(0.0f, 0.0f);
	sceneObjects["ScoreQuad17"]->uUVOffset = glm::vec2(0.0f, 0.0f);
	sceneObjects["ScoreQuad18"]->uUVOffset = colonPos;
	sceneObjects["ScoreQuad19"]->uUVOffset = glm::vec2(0.0f, 0.0f);
	sceneObjects["ScoreQuad20"]->uUVOffset = glm::vec2(0.0f, 0.0f);

	sceneObjects["ScoreQuad21"]->uUVOffset = glm::vec2(0.0f, 0.0f);
	sceneObjects["ScoreQuad22"]->uUVOffset = glm::vec2(0.0f, 0.0f);
	sceneObjects["ScoreQuad23"]->uUVOffset = colonPos;
	sceneObjects["ScoreQuad24"]->uUVOffset = glm::vec2(0.0f, 0.0f);
	sceneObjects["ScoreQuad25"]->uUVOffset = glm::vec2(0.0f, 0.0f);

	sceneObjects["ScoreQuad26"]->uUVOffset = glm::vec2(0.0f, 0.0f);
	sceneObjects["ScoreQuad27"]->uUVOffset = glm::vec2(0.0f, 0.0f);
	sceneObjects["ScoreQuad28"]->uUVOffset = colonPos;
	sceneObjects["ScoreQuad29"]->uUVOffset = glm::vec2(0.0f, 0.0f);
	sceneObjects["ScoreQuad30"]->uUVOffset = glm::vec2(0.0f, 0.0f);

	sceneObjects["ScoreH"]->uUVOffset = glm::vec2(0.0f, 0.0f);
	sceneObjects["ScoreI"]->uUVOffset = glm::vec2(0.0f, 0.0f);
	sceneObjects["ScoreG"]->uUVOffset = glm::vec2(0.0f, 0.0f);
	sceneObjects["ScoreH2"]->uUVOffset = glm::vec2(0.0f, 0.0f);
	sceneObjects["ScoreS"]->uUVOffset = glm::vec2(0.0f, 0.0f);
	sceneObjects["ScoreC"]->uUVOffset = glm::vec2(0.0f, 0.0f);
	sceneObjects["ScoreO"]->uUVOffset = glm::vec2(0.0f, 0.0f);
	sceneObjects["ScoreR"]->uUVOffset = glm::vec2(0.0f, 0.0f);
	sceneObjects["ScoreE"]->uUVOffset = glm::vec2(0.0f, 0.0f);
	sceneObjects["ScoreS2"]->uUVOffset = glm::vec2(0.0f, 0.0f);

	sceneObjects["ScoreQuad26"]->uUVOffset = onePos;
	sceneObjects["ScoreQuad27"]->uUVOffset = twoPos;
	sceneObjects["ScoreQuad28"]->uUVOffset = threePos;
	sceneObjects["ScoreQuad29"]->uUVOffset = fourPos;
	sceneObjects["ScoreQuad30"]->uUVOffset = fivePos;

	sceneObjects["ScoreH"]->uUVOffset = HPos;
	sceneObjects["ScoreI"]->uUVOffset = IPos;
	sceneObjects["ScoreG"]->uUVOffset = GPos;
	sceneObjects["ScoreH2"]->uUVOffset = HPos;
	sceneObjects["ScoreS"]->uUVOffset = SPos;
	sceneObjects["ScoreC"]->uUVOffset = CPos;
	sceneObjects["ScoreO"]->uUVOffset = OPos;
	sceneObjects["ScoreR"]->uUVOffset = RPos;
	sceneObjects["ScoreE"]->uUVOffset = EPos;
	sceneObjects["ScoreS2"]->uUVOffset = SPos;

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
	lightObjects.push_back(&roomLight);
	
	if (!hasLoadedOnce)
	{
		pointLight.color = glm::vec4(0.8f, 0.4f, 0.69f, 1.0f);
		pointLight.meshName = "Cone";

		pointLight2.color = glm::vec4(0.22f, 0.84f, 1.0f, 1.0f);
		pointLight2.meshName = "Cone";

		pointLight3.color = glm::vec4(0.3f, 0.7f, 0.4f, 1.0f);
		pointLight3.aConstant = pointLight3.aConstant * 0.1f;
		pointLight3.aLinear = pointLight3.aLinear * 0.1f;
		pointLight3.aQuadratic = pointLight3.aQuadratic * 0.1f;
		pointLight3.meshName = "Pyramid";

		pointLight4.color = glm::vec4(0.3f, 0.7f, 0.4f, 1.0f);
		pointLight4.aConstant = pointLight4.aConstant * 0.1f;
		pointLight4.aLinear = pointLight4.aLinear * 0.1f;
		pointLight4.aQuadratic = pointLight4.aQuadratic * 0.1f;
		pointLight4.meshName = "Pyramid";

		pointLight5.color = glm::vec4(0.3f, 0.7f, 0.4f, 1.0f);
		pointLight5.aConstant = pointLight5.aConstant * 0.1f;
		pointLight5.aLinear = pointLight5.aLinear * 0.1f;
		pointLight5.aQuadratic = pointLight5.aQuadratic * 0.1f;
		pointLight5.meshName = "Pyramid";

		candleLight.color = glm::vec4(1.0f, 0.67f, 0.02f, 1.0f);
		candleLight.position = glm::vec3(-32.5f, 10.0f, 17.5f);
		candleLight.meshName = "Sphere";
		candleLight.aConstant = candleLight.aConstant * 2.0f;
		candleLight.aLinear = candleLight.aLinear * 2.0f;
		candleLight.aQuadratic = candleLight.aQuadratic * 2.0f;

		candleLight2 = candleLight;
		candleLight2.position = glm::vec3(-32.5f, 10.0f, -17.0f);
		
		candleLight3 = candleLight;
		candleLight3.position = glm::vec3(32.5f, 10.0f, 17.0f);

		candleLight4 = candleLight;
		candleLight4.position = glm::vec3(32.5f, 10.0f, -17.0f);

		candleLight5 = candleLight;
		candleLight5.position = glm::vec3(17.0f, 10.0f, 32.5f);

		candleLight6 = candleLight;
		candleLight6.position = glm::vec3(-17.0f, 10.0f, 32.5f);

		candleLight7 = candleLight;
		candleLight7.position = glm::vec3(17.0f, 10.0f, -32.5f);

		candleLight8 = candleLight;
		candleLight8.position = glm::vec3(-17.0f, 10.0f, -32.5f);

		roomLight.position = glm::vec3(0.0f, 50.0f, 0.0f);
		roomLight.color = glm::vec4(0.1f, 0.1f, 0.2f, 2.0f);
		roomLight.aConstant = roomLight.aConstant * 0.1f;
		roomLight.aLinear = roomLight.aLinear * 0.1f;
		roomLight.aQuadratic = roomLight.aQuadratic * 0.1f;
		roomLight.meshName = "Pyramid";

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
	Sounds["dead"]->channel->stop();
	Sounds["candle"]->channel->stop();

	hasBeenInitialized = false;
	if (m_paused == false)
	{
		m_paused = true;
		GameLevel::SetPaused(m_paused);
		MainMenu(hasBeenInitialized);
	}

}

//GAME OVER SCREEN MAIN LOOP
void Tutorial::Update()
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
		Tutorial::exit();
		m_parent->GetGameState("GameLevel")->SetPaused(false);
	}

	ENG::Input::ResetKeys();
}

Tutorial::Tutorial()
{
	hasBeenInitialized = false;
}

void Tutorial::enter()
{
	if (m_paused == true)
	{
		m_paused = false;
		Tutorial::SetPaused(m_paused);
	}

	gObjects.push_back(sceneObjects["Quad2"]);
	sceneObjects["Quad2"]->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	hasLoadedOnce = true;
}

void Tutorial::exit()
{
	removeGameObjects();

	hasBeenInitialized = false;
	if (m_paused == false)
	{
		m_paused = true;
		Tutorial::SetPaused(m_paused);
		GameLevel(hasBeenInitialized);
	}
}

float timerFunc(float deltaTime)
{
	seconds += deltaTime;

	if (seconds >= 60.0f)
	{
		seconds = 0.0f;
		minute += 1;
	}

	secondsPos = int(seconds) % 10;

	decondsPos = int(seconds / 10);

	minutesPos = int(minute) % 10;

	dinutesPos = int(minute / 10);

	return seconds;

}

void interpretScore()
{
	score.readScore();
	
	int firstDigit = int(score.timeScore[0]/ 600) % 10;
	int secondDigit = int(score.timeScore[0]/ 60) % 10;
	int thirdDigit = int(score.timeScore[0]/ 10) % 6;
	int fourthDigit = int(score.timeScore[0]) % 10;

	sceneObjects["ScoreQuad"]->uUVOffset = sceneObjects["TimerQuad"]->UVOffsets[firstDigit];
	sceneObjects["ScoreQuad2"]->uUVOffset = sceneObjects["TimerQuad"]->UVOffsets[secondDigit];
	sceneObjects["ScoreQuad4"]->uUVOffset = sceneObjects["TimerQuad"]->UVOffsets[thirdDigit];
	sceneObjects["ScoreQuad5"]->uUVOffset = sceneObjects["TimerQuad"]->UVOffsets[fourthDigit];

	firstDigit = int(score.timeScore[1] / 600) % 10;
	secondDigit = int(score.timeScore[1] / 60) % 10;
	thirdDigit = int(score.timeScore[1] / 10) % 6;
	fourthDigit = int(score.timeScore[1]) % 10;

	sceneObjects["ScoreQuad6"]->uUVOffset = sceneObjects["TimerQuad"]->UVOffsets[firstDigit];
	sceneObjects["ScoreQuad7"]->uUVOffset = sceneObjects["TimerQuad"]->UVOffsets[secondDigit];
	sceneObjects["ScoreQuad9"]->uUVOffset = sceneObjects["TimerQuad"]->UVOffsets[thirdDigit];
	sceneObjects["ScoreQuad10"]->uUVOffset = sceneObjects["TimerQuad"]->UVOffsets[fourthDigit];

	firstDigit = int(score.timeScore[2] / 600) % 10;
	secondDigit = int(score.timeScore[2] / 60) % 10;
	thirdDigit = int(score.timeScore[2] / 10) % 6;
	fourthDigit = int(score.timeScore[2]) % 10;

	sceneObjects["ScoreQuad11"]->uUVOffset = sceneObjects["TimerQuad"]->UVOffsets[firstDigit];
	sceneObjects["ScoreQuad12"]->uUVOffset = sceneObjects["TimerQuad"]->UVOffsets[secondDigit];
	sceneObjects["ScoreQuad14"]->uUVOffset = sceneObjects["TimerQuad"]->UVOffsets[thirdDigit];
	sceneObjects["ScoreQuad15"]->uUVOffset = sceneObjects["TimerQuad"]->UVOffsets[fourthDigit];

	firstDigit = int(score.timeScore[3] / 600) % 10;
	secondDigit = int(score.timeScore[3] / 60) % 10;
	thirdDigit = int(score.timeScore[3] / 10) % 6;
	fourthDigit = int(score.timeScore[3]) % 10;

	sceneObjects["ScoreQuad16"]->uUVOffset = sceneObjects["TimerQuad"]->UVOffsets[firstDigit];
	sceneObjects["ScoreQuad17"]->uUVOffset = sceneObjects["TimerQuad"]->UVOffsets[secondDigit];
	sceneObjects["ScoreQuad19"]->uUVOffset = sceneObjects["TimerQuad"]->UVOffsets[thirdDigit];
	sceneObjects["ScoreQuad20"]->uUVOffset = sceneObjects["TimerQuad"]->UVOffsets[fourthDigit];

	firstDigit = int(score.timeScore[4] / 600) % 10;
	secondDigit = int(score.timeScore[4] / 60) % 10;
	thirdDigit = int(score.timeScore[4] / 10) % 6;
	fourthDigit = int(score.timeScore[4]) % 10;

	sceneObjects["ScoreQuad21"]->uUVOffset = sceneObjects["TimerQuad"]->UVOffsets[firstDigit];
	sceneObjects["ScoreQuad22"]->uUVOffset = sceneObjects["TimerQuad"]->UVOffsets[secondDigit];
	sceneObjects["ScoreQuad24"]->uUVOffset = sceneObjects["TimerQuad"]->UVOffsets[thirdDigit];
	sceneObjects["ScoreQuad25"]->uUVOffset = sceneObjects["TimerQuad"]->UVOffsets[fourthDigit];
}

void removeGameObjects()
{
	gObjects.clear();
	transparentGObjects.clear();
	HUDGObjects.clearList();
}