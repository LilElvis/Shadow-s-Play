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
private:
	bool systemInit;
public:
	SoundEngine();
	~SoundEngine();
	void Update();
	bool Init();

	FMOD::System *system;
	void         *driverData;

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
	bool load(char*, bool);
	void play();
	static void systemUpdate();

	FMOD::Sound     *sound;
	FMOD::Channel   *channel;
	FMOD_VECTOR pos;
	FMOD_VECTOR vel;

	static SoundEngine Sys;
};
