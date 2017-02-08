//RAYMOND DEE - 100578667
#include "Sound.h"

//FOR FUTURE FMOD IMPLEMENTATION
FMOD_RESULT result;

void CheckResult(FMOD_RESULT result)
{
	if (result != FMOD_OK)
	{
		std::string mesg = FMOD_ErrorString(result);
	}
}

SoundEngine::SoundEngine()
{
	system = NULL;
	driverData = NULL;

	systemInit = false;

	listenerForward = { 0.0f, 0.0f,  1.0f };
	listenerUp = { 0.0f, 1.0f,  0.0f };
	listenerPos = { 0.0f, 0.0f,  0.0f };
	listenerVel = { 0.0f, 0.0f,  0.0f };
}

bool SoundEngine::Init()
{
	if (systemInit) return true;

	//Create a System object and initialize.
	unsigned int version;
	result = FMOD::System_Create(&system); CheckResult(result);
	result = system->getVersion(&version); CheckResult(result);

	if (version < FMOD_VERSION)
	{
		std::cout << "FMOD lib version doesn't match header" << std::endl;
		return false;
	}

	result = system->init(100, FMOD_INIT_NORMAL, driverData);
	CheckResult(result);
	if (result != FMOD_OK)
	{
		std::cout << "Initialization Failed" << std::endl;
		return false;
	}

	result = system->set3DSettings(1.0, 1.0f, 1.0f);
	CheckResult(result);

	systemInit = true;
	return true;
}

SoundEngine::~SoundEngine()
{
	if (systemInit)
	{
		result = system->close(); CheckResult(result);
		result = system->release(); CheckResult(result);
	}
}

void SoundEngine::Update()
{
	result = system->set3DListenerAttributes(0, &listenerPos, &listenerVel, &listenerForward, &listenerUp);
	CheckResult(result);

	result = system->update(); CheckResult(result);
}

SoundEngine Sound::Sys;

Sound::Sound()
{
	sound = NULL;
	channel = NULL;
	pos = { 0.0f, 0.0f, 0.0f };
	vel = { 0.0f, 0.0f, 0.0f };
}

Sound::~Sound()
{
	result = sound->release(); CheckResult(result);
}

bool Sound::load(char* filename, bool loop)
{
	Sys.Init();

	result = Sys.system->createSound(filename, FMOD_3D, 0, &sound);
	CheckResult(result);

	if (result != FMOD_OK)
	{
		std::cout << "Couldn't open file: " << filename << std::endl;
		return false;
	}
	result = sound->set3DMinMaxDistance(0.5f, 5000.0f); CheckResult(result);

	if (loop)
	{
		result = sound->setMode(FMOD_LOOP_NORMAL); CheckResult(result);
	}
	if (!loop)
	{
		result = sound->setMode(FMOD_LOOP_OFF); CheckResult(result);
	}

	return true;
}
void Sound::play()
{
	result = Sys.system->playSound(sound, 0, true, &channel); CheckResult(result);
	result = channel->set3DAttributes(&pos, &vel); CheckResult(result);
	result = channel->setPaused(false); CheckResult(result);
}

void Sound::systemUpdate()
{
	Sys.Update();
}
