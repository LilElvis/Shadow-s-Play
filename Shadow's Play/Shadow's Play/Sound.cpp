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

	init = false;

	listenerForward = { 0.0f, 0.0f,  1.0f };
	listenerUp = { 0.0f, 1.0f,  0.0f };
	listenerPos = { 0.0f, 0.0f,  0.0f };
	listenerVel = { 0.0f, 0.0f,  0.0f };
}

bool SoundEngine::Init()
{
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

	init = true;
	return true;
}

SoundEngine::~SoundEngine()
{
	result = system->close(); CheckResult(result);
	result = system->release(); CheckResult(result);
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
}

Sound::~Sound()
{
	result = sound->release(); CheckResult(result);
}

bool Sound::load(char* fileName, bool is3d, bool isLoop)
{
	if (!Sys.init)
	{
		if (!Sys.Init()) return false;
	}

	//Load Sounds
	this->is3d = is3d;
	if (is3d)
	{
		result = Sys.system->createSound(fileName, FMOD_3D, 0, &sound);
	}
	else
	{
		result = Sys.system->createSound(fileName, FMOD_2D, 0, &sound);
	}
	CheckResult(result);
	if (result != FMOD_OK)
	{
		std::cout << "Couldn't open file: " << fileName << std::endl;
		return false;
	}
	result = sound->set3DMinMaxDistance(0.5f, 5000.0f); CheckResult(result);

	if (isLoop)
	{
		sound->setMode(FMOD_LOOP_NORMAL);
	}
	if (!isLoop)
	{
		sound->setMode(FMOD_LOOP_OFF);
	}

	return true;
}
FMOD::Channel* Sound::play()
{
	FMOD_VECTOR pos = { 0.0f, 0.0f, 0.0f };
	FMOD_VECTOR vel = { 0.0f, 0.0f, 0.0f };
	
	//Play Sounds
	result = Sys.system->playSound(sound, 0, true, &channel); CheckResult(result);
	if (is3d)
	{
		result = channel->set3DAttributes(&pos, &vel); CheckResult(result);
	}
	result = channel->setPaused(false); CheckResult(result);

	return channel;
}

void Sound::systemUpdate()
{
	Sys.Update();
}

void Sound::setPosition(FMOD::Channel *channel, FMOD_VECTOR position, FMOD_VECTOR velocity)
{
	if (is3d)
	{
		result = channel->set3DAttributes(&position, &velocity); CheckResult(result);
	}
}

void Sound::setRolloff(FMOD::Channel *channel, bool linear, float min, float max)
{
	channel->set3DMinMaxDistance(min, max);
	if (linear)
	{
		channel->setMode(FMOD_3D_LINEARROLLOFF);
	}
	else
	{
		channel->setMode(FMOD_3D_INVERSEROLLOFF);
	}
}
