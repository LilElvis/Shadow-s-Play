//Cameron van Velzen - 100591663
#pragma once
#include "GameStates.h"
#include "Window.h"
#include "Texture.h"
#include "Transform.h"
#include "AnimationMath.h"
#include "FrameBufferObject.h"
#include "DLList.h"

class GameLevel : public GameState
{
public:
	GameLevel();
	void enter();
	void Update();
private:
	std::vector<ENG::GameObject*> collidables;

	bool hasBeenInitialized;
	bool hasLoadedOnce = false;
	bool litRotation = false;
	bool Nyx1Active = false;
	bool Nyx2Active = false;
	bool Nyx3Active = false;
	bool Nyx4Active = false;
	bool Nyx5Active = false;
};

void Initialize();
void Reset();

//WINDOW STUFF
static sf::ContextSettings Settings;
static int windowWidth = 1778;
static int windowHeight = 1000;
static float aspectRatio = windowWidth / float(windowHeight);

//DECLARE SINGLETON POINTERS
static ENG::Window* gameWindow = ENG::Window::gameInstance();
static ENG::Texture* defaultTexture = ENG::Texture::getTexPtr();
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
static ENG::Shader bokehBlur;
static ENG::Shader bokehComposite;

//FBOS
static ENG::FrameBufferObject geometryBuffer;
static ENG::FrameBufferObject finalSceneFBO1;
static ENG::FrameBufferObject finalSceneFBO2;
static ENG::FrameBufferObject bloomFBO1;
static ENG::FrameBufferObject bloomFBO2;
static ENG::FrameBufferObject motionBlurFBO;
static ENG::FrameBufferObject lightingStencil;
static ENG::FrameBufferObject bokehA;
static ENG::FrameBufferObject bokehB;
static ENG::FrameBufferObject bokehHorBlur;

//MAPS OF GAME OBJECTS
static std::vector <ENG::GameObject*> gObjects;
static std::vector <ENG::GameObject*> transparentGObjects;
static ENG::DLList HUDGObjects;
static std::vector <ENG::PointLight*> lightObjects;

static void removeGameObjects(); //FLUSHES VECTORS OF GAME OBJECTS

static std::unordered_map<std::string, ENG::SceneObject*> sceneObjects;
static std::unordered_map<std::string, ENG::SceneObject*>::iterator sceneObjects_iterator;
static std::unordered_map<std::string, sf::Sprite> sfSprites;

static glm::vec3 points[26];

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

static float globalT = 1.0f;
static float rampValue = 0.005f;

//TEMP ANIMATION VARIABLES
static unsigned int animFrame = 0;
static float timeOfLastAnim = 0.0f;

//PAUSE
static bool weBePausing = false;

static ENG::PointLight roomLight;
static ENG::PointLight roomLight2;
static ENG::PointLight roomLight3;

//TOGGLE MODES
static bool TEXTURE_TOGGLE = true;
static bool LIGHTING_TOGGLE = true;
static bool BLUR_TOGGLE = true;

enum LUT_TOGGLE
{
	ASPEN,
	BOURBON,
	CLAYTON,
	ZEKE,
	PITAYA
};

static LUT_TOGGLE LUT_TOGGLES;