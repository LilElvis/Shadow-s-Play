#pragma once
#include "GameStates.h"
#include "Window.h"
#include "Sound.h"
#include "Texture.h"
#include "Transform.h"
#include "Player.h"
#include "AnimationMath.h"
#include "FrameBufferObject.h"
#include "LUT.h"
#include "Score.h"
#include "DLList.h"

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
static ENG::Shader lightingComposite;
static ENG::Shader deferredLighting;
static ENG::Shader bloomHighPass;
static ENG::Shader bloomHorizontalBlur;
static ENG::Shader bloomVerticalBlur;
static ENG::Shader bloomComposite;
static ENG::Shader UVScrolling;
static ENG::Shader motionBlur;
static ENG::Shader colorCorrect;

//FBOS
static ENG::FrameBufferObject geometryBuffer;
static ENG::FrameBufferObject finalSceneFBO1;
static ENG::FrameBufferObject finalSceneFBO2;
static ENG::FrameBufferObject bloomFBO1;
static ENG::FrameBufferObject bloomFBO2;
static ENG::FrameBufferObject motionBlurFBO;
static ENG::FrameBufferObject lightingStencil;

//LUTS
static ENG::LUT bourbon;
static ENG::LUT clayton;


//MAPS OF GAME OBJECTS
static std::vector <ENG::GameObject*> gObjects;
static std::vector <ENG::GameObject*> transparentGObjects;
static ENG::DLList HUDGObjects;
static std::vector <ENG::PointLight*> lightObjects;

static void removeGameObjects(); //FLUSHES VECTORS OF GAME OBJECTS

static std::unordered_map<std::string, ENG::Player*> Player;
static std::unordered_map<std::string, ENG::SceneObject*> sceneObjects;
static std::unordered_map<std::string, ENG::SceneObject*>::iterator sceneObjects_iterator;
static std::unordered_map<std::string, sf::Sprite> sfSprites;

static glm::vec3 points[26];

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

static glm::vec4 defaultLightPos;

//TIME VARIABLES
static float totalTime = 0.0f;		//Total Runtime
static float sessionTime = 0.0f;	//Current Play Session Runtime
static float previousTime = 0.0f;
static float deltaTime = 0.0f;

static float timeOfDeath = -1.0f;
static bool deathTimer = false;

static float seconds = 0;
static int minute = 0;
static int numOfCycles = 0;

static ENG::Score score;

//TIMER FUNC
float timerFunc(float deltaTime);
static unsigned int secondsPos = 0;
static unsigned int decondsPos = 0;
static unsigned int minutesPos = 0;
static unsigned int dinutesPos = 0;

static glm::vec2 zeroPos = glm::vec2(0.0f, 0.0f);
static glm::vec2 onePos = glm::vec2(0.1f, 0.0f);
static glm::vec2 twoPos = glm::vec2(0.2f, 0.0f);
static glm::vec2 threePos = glm::vec2(0.3f, 0.0f);
static glm::vec2 fourPos = glm::vec2(0.4f, 0.0f);
static glm::vec2 fivePos = glm::vec2(0.5f, 0.0f);
static glm::vec2 sixPos = glm::vec2(0.6f, 0.0f);
static glm::vec2 sevenPos = glm::vec2(0.7f, 0.0f);
static glm::vec2 eightPos = glm::vec2(0.8f, 0.0f);
static glm::vec2 ninePos = glm::vec2(0.9f, 0.0f);
static glm::vec2 APos = glm::vec2(0.0f, -0.1f);
static glm::vec2 BPos = glm::vec2(0.1f, -0.1f);
static glm::vec2 CPos = glm::vec2(0.2f, -0.1f);
static glm::vec2 DPos = glm::vec2(0.3f, -0.1f);
static glm::vec2 EPos = glm::vec2(0.4f, -0.1f);
static glm::vec2 FPos = glm::vec2(0.5f, -0.1f);
static glm::vec2 GPos = glm::vec2(0.6f, -0.1f);
static glm::vec2 HPos = glm::vec2(0.7f, -0.1f);
static glm::vec2 IPos = glm::vec2(0.8f, -0.1f);
static glm::vec2 JPos = glm::vec2(0.9f, -0.1f);
static glm::vec2 KPos = glm::vec2(0.0f, -0.2f);
static glm::vec2 LPos = glm::vec2(0.1f, -0.2f);
static glm::vec2 MPos = glm::vec2(0.2f, -0.2f);
static glm::vec2 NPos = glm::vec2(0.3f, -0.2f);
static glm::vec2 OPos = glm::vec2(0.4f, -0.2f);
static glm::vec2 PPos = glm::vec2(0.5f, -0.2f);
static glm::vec2 QPos = glm::vec2(0.6f, -0.2f);
static glm::vec2 RPos = glm::vec2(0.7f, -0.2f);
static glm::vec2 SPos = glm::vec2(0.8f, -0.2f);
static glm::vec2 TPos = glm::vec2(0.9f, -0.2f);
static glm::vec2 UPos = glm::vec2(0.0f, -0.3f);
static glm::vec2 VPos = glm::vec2(0.1f, -0.3f);
static glm::vec2 WPos = glm::vec2(0.2f, -0.3f);
static glm::vec2 XPos = glm::vec2(0.3f, -0.3f);
static glm::vec2 YPos = glm::vec2(0.4f, -0.3f);
static glm::vec2 ZPos = glm::vec2(0.5f, -0.3f);
static glm::vec2 colonPos = glm::vec2(0.6f, -0.3f);

static float globalT = 1.0f;
static float rampValue = 0.005f;

//TEMP ANIMATION VARIABLES
static unsigned int animFrame = 0;
static float timeOfLastAnim = 0.0f;

//RANDOM VARIABLES
static int randomLERPStart = 0;
static int randomLERPEnd = 0;

static int randomCurveStart = 0;
static int randomCurveEnd = 0;
static int randomCurveControl = 0;

static int randomQuadPos = 0;
static int randomTriPos = 0;
static int randomTriPos2 = 0;

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
static ENG::PointLight pointLight4;
static ENG::PointLight pointLight5;

static ENG::PointLight candleLight;
static ENG::PointLight candleLight2;
static ENG::PointLight candleLight3;
static ENG::PointLight candleLight4;
static ENG::PointLight candleLight5;
static ENG::PointLight candleLight6;
static ENG::PointLight candleLight7;
static ENG::PointLight candleLight8;

//static glm::mat4 uProjectionBiasMatrixInverse;