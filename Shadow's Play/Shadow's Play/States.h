#pragma once
#include "GameStates.h"
#include "Window.h"
#include "Sound.h"
#include "Texture.h"
#include "Transform.h"
#include "Player.h"
#include "SceneObject.h"
#include "AnimationMath.h"
#include "FrameBufferObject.h"

//STATE CLASSES
class MainMenu : public GameState
{
public:
	MainMenu();
	void enter();
	void exit();
	void Update();
private:	
	std::vector<ENG::GameObject*> collidables;

	bool hasBeenInitialized;
	bool hasLoadedOnce = false;
};

class GameLevel : public GameState
{
public:
	GameLevel();
	void enter();
	//void exit();
	void gameOver(); //is now exit();
	void Update();
private:
	std::vector<ENG::GameObject*> collidables;

	bool hasBeenInitialized;
	bool hasLoadedOnce = false;
};

//class TwoPlayerLevel : public GameState
//{
//public:
//	LevelTwo();
//	void enter();
//	void exit();
//	void gameOver();
//	void Update();
//private:
//	std::vector<ENG::GameObject*> collidables;
//
//	bool hasBeenInitialized;
//};

class GameOver : public GameState
{
public:
	GameOver();
	void enter();
	void exit();
	void Update();
private:
	bool hasBeenInitialized;
	bool hasLoadedOnce = false;
};

void Initialize();
void Reset();

//WINDOW STUFF
static sf::ContextSettings Settings;
static int windowWidth = 1600;
static int windowHeight = 911;
static float aspectRatio = windowWidth / float(windowHeight);

//DECLARE SINGLETON POINTERS
static ENG::Window* gameWindow = ENG::Window::gameInstance();
static ENG::Texture* defaultTexture = ENG::Texture::getTexPtr();
static SoundEngine soundEngine;
static ENG::Mesh* defaultMesh = ENG::Mesh::getMeshPtr();

//SHADER PROGRAMS
static ENG::Shader defaultShader;
static ENG::Shader passThrough;
static ENG::Shader GBuffer;
static ENG::Shader deferredShading;
static ENG::Shader bloomHighPass;
static ENG::Shader bloomHorizontalBlur;
static ENG::Shader bloomVerticalBlur;
static ENG::Shader bloomComposite;
static ENG::Shader UVScrolling;
static ENG::Shader motionBlur;

//FBOS
static ENG::FrameBufferObject deferredFBO;
static ENG::FrameBufferObject finalSceneFBO1;
static ENG::FrameBufferObject finalSceneFBO2;
static ENG::FrameBufferObject bloomFBO1;
static ENG::FrameBufferObject bloomFBO2;
static ENG::FrameBufferObject motionBlurFBO;


//MAPS OF GAME OBJECTS
static std::vector <ENG::GameObject*> gObjects;
static std::vector <ENG::GameObject*> transparentGObjects;
static std::vector <ENG::GameObject*> HUDGObjects;

static void removeGameObjects(); //FLUSHES VECTORS OF GAME OBJECTS

static std::unordered_map<std::string, ENG::Player*> Player;
static std::unordered_map<std::string, ENG::SceneObject*> sceneObjects;
static std::unordered_map<std::string, ENG::SceneObject*>::iterator sceneObjects_iterator;
static std::unordered_map<std::string, sf::Sprite> sfSprites;

static glm::vec3 points[17];

//SOUNDS (FMOD VARIABLES/CHANNELS/VECTORS/DSP's)
static std::unordered_map<std::string, Sound*> Sounds;

static FMOD::Channel *mainMenuChannel;
static FMOD::Channel *bgmChannel;
static FMOD::Channel *dieChannel;
static FMOD::Channel *warningChannel;
static FMOD::Channel *pauseMenuChannel;
static FMOD::Channel *dashChannel;

static FMOD_VECTOR playerPos;
static FMOD_VECTOR playerVel;
static FMOD_VECTOR diePos;
static FMOD_VECTOR dieVel;
static FMOD_VECTOR warningPos;
static FMOD_VECTOR warningVel;

static FMOD::DSP *pitchShift;
static FMOD::DSP *lowPass;
static FMOD::DSP *echo;

static float defaultVolume;

static bool hasBeenWarned = false;
static bool inRadius = false;

//DEV CONTROL INPUT HANDLE
static ENG::Input devCommand;

//DECLARE CAMERA VECTORS
static glm::vec3 cameraPos;

static glm::vec3 cameraTarget;
static glm::vec3 cameraDirection;

static glm::vec3 up;
static glm::vec4 down;
static glm::vec3 cameraRight;

static glm::vec3 cameraUp;

static glm::vec3 cameraFront;

static ENG::Transform view;

static glm::mat4 persp;
static glm::mat4 orthoPersp;

static glm::mat4 viewInverse;
static glm::vec3 updatedCamPos;

static bool isLit;
static bool wasWarned1 = false;
static bool wasWarned2 = false;

//TIME VARIABLES
static float totalTime = 0.0f;
static float previousTime = 0.0f;
static float deltaTime = 0.0f;

static float timeOfDeath = 0.0f;
static bool deathTimer = false;

static float seconds = 0;
static int minute = 0;

//TIMER FUNC
float timerFunc();
static glm::vec2 secondsPos = glm::vec2(0.0f, 0.0f);
static glm::vec2 decondsPos = glm::vec2(0.0f, 0.0f);
static glm::vec2 minutesPos = glm::vec2(0.0f, 0.0f);
static glm::vec2 dinutesPos = glm::vec2(0.0f, 0.0f);

static glm::vec2 zeroPos = glm::vec2(0.0f, 0.0f);
static glm::vec2 onePos = glm::vec2(0.2f, 0.0f);
static glm::vec2 twoPos = glm::vec2(0.4f, 0.0f);
static glm::vec2 threePos = glm::vec2(0.6f, 0.0f);
static glm::vec2 fourPos = glm::vec2(0.8f, 0.0f);
static glm::vec2 fivePos = glm::vec2(0.0f, -0.2f);
static glm::vec2 sixPos = glm::vec2(0.2f, -0.2f);
static glm::vec2 sevenPos = glm::vec2(0.4f, -0.2f);
static glm::vec2 eightPos = glm::vec2(0.6f, -0.2f);
static glm::vec2 ninePos = glm::vec2(0.8f, -0.2f);
static glm::vec2 colonPos = glm::vec2(0.0f, -0.4f);

static float globalT = 1.0f;
static float rampValue = 0.005f;

//RANDOM VARIABLES
static int randomLERPStart = 0;
static int randomLERPEnd = 0;

static int randomCurveStart = 0;
static int randomCurveEnd = 0;
static int randomCurveControl = 0;

static int randomQuadPos = 0;

//PAUSE
static bool weBePausing = false;
static bool pressed = sf::Joystick::isButtonPressed(0, 7);

//BOUNDRAIES
static float bounds = 25.0f;
static glm::vec3 RoomMax = glm::vec3(bounds, 10.5f, bounds);
static glm::vec3 RoomMin = glm::vec3(-bounds, 10.0f, -bounds);

//LIGHTS
static ENG::PointLight pointLight;
static ENG::PointLight pointLight2;
static ENG::PointLight pointLight3;

static ENG::PointLight warningLight;
static ENG::PointLight warningLight2;
static ENG::PointLight warningLight3;

static float flashValue;

//static glm::mat4 uProjectionBiasMatrixInverse;