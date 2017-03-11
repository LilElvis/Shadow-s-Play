//RAYMOND DEE - 100578667
#pragma once

#include "Window.h"
#include <vector>


#include "FMOD/fmod.hpp"
#include "FMOD/fmod_errors.h"
#include <Windows.h>
#include <iostream>

void CheckResult(FMOD_RESULT result);

class SoundEngine
{
public:
	SoundEngine();
	~SoundEngine();
	void Update();
	bool Init();

	FMOD::System *system;
	void         *driverData;
	bool init;

	//Listener
	FMOD_VECTOR listenerForward;
	FMOD_VECTOR listenerUp;
	FMOD_VECTOR listenerPos;
	FMOD_VECTOR listenerVel;
};

class Sound
{
public:
	Sound();
	~Sound();
	bool load(char* fileName, bool is3d, bool isLoop);
	FMOD::Channel* play();
	void setPosition(FMOD::Channel *channel, FMOD_VECTOR position, FMOD_VECTOR velocity);
	void setRolloff(FMOD::Channel *channel, bool linear, float min, float max);
	static void systemUpdate();

	FMOD::Sound     *sound;
	FMOD::Channel   *channel;
	bool is3d;

	static SoundEngine Sys;
};
