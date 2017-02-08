#pragma once
#include "GameStates.h"
#include "Window.h"
#include "Sound.h"
#include "Texture.h"
#include "Transform.h"
#include "Player.h"
#include "SceneObject.h"
#include "AnimationMath.h"

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

static sf::ContextSettings Settings;

//DECLARE SINGLETON POINTERS
static ENG::Window* gameWindow = ENG::Window::gameInstance();
static ENG::Texture* defaultTexture = ENG::Texture::getTexPtr();
static SoundEngine soundEngine;
static ENG::Mesh* defaultMesh = ENG::Mesh::getMeshPtr();

//SHADER PROGRAMS
static ENG::Shader defaultShader;
static ENG::Shader passThrough;

//MAPS OF GAME OBJECTS
static std::unordered_map<std::string, ENG::Player*> Player;
static std::unordered_map<std::string, ENG::SceneObject*> sceneObjects;
static std::unordered_map<std::string, ENG::SceneObject*>::iterator sceneObjects_iterator;
static std::unordered_map<std::string, sf::Sprite> sfSprites;
static std::unordered_map<std::string, Sound*> Sounds;

static glm::vec3 points[17];

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

static bool isLit;
static bool wasWarned1 = false;
static bool wasWarned2 = false;

//TIME VARIABLES
static float totalTime = 0.0f;
static float previousTime = 0.0f;
static float deltaTime = 0.0f;

static float globalT = 1.0f;
static float rampValue = 0.005f;

//RANDOM VARIABLES
static int randomLERPStart = 0;
static int randomLERPEnd = 0;

static int randomCurveStart = 0;
static int randomCurveEnd = 0;
static int randomCurveControl = 0;

static int randomQuadPos = 0;

//BOUNDRAIES
static float bounds = 25.0f;
static glm::vec3 RoomMax = glm::vec3(bounds, 10.5f, bounds);
static glm::vec3 RoomMin = glm::vec3(-bounds, 10.0f, -bounds);