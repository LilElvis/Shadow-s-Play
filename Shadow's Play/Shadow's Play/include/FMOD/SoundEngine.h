//Raymond Dee Jan. 2017

#pragma once
#pragma comment(lib, "FMOD/fmod_vc.lib") //Visual Studio Specific include line

#include "fmod.hpp"
#include "fmod_errors.h"
#include <Windows.h>
#include <iostream> //cout

using namespace std;

void CheckResult(FMOD_RESULT result);

class SoundEngine
{
private:
	bool systemInit;
public:
	SoundEngine();
	bool Init();
	void Update();

	//Listener
	FMOD_VECTOR listenerForward;
	FMOD_VECTOR listenerUp;
	FMOD_VECTOR listenerPos;
	FMOD_VECTOR listenerVel;

	FMOD::System *system;
	void		*driverData;
};

class Sound
{
public:
	Sound();
	~Sound();
	bool Load(char *);
	void Play();

	//Sound
	FMOD::Sound   *sound;
	FMOD::Channel *channel;
	FMOD_VECTOR pos;
	FMOD_VECTOR vel;

	static SoundEngine Sys;
};