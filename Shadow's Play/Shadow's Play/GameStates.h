#pragma once
#include <iostream> 
#include <string>
#include <unordered_map>

class GameState;

//CONTAINS AND MANAGES SWAPPING OF STATES
class GameStateManager
{
public:
	void AddGameState(std::string a_string, GameState* a_state);
	void RemoveGameState(std::string a_string);
	GameState* GetGameState(std::string a_string);
	void Update();
private:
	std::unordered_map<std::string, GameState*> m_states;
};

//PARENT CLASS FOR ALL STATES
class GameState
{
public:
	virtual void Update() = 0;
	void SetPaused(bool a_paused);

	bool m_paused = false;

	GameStateManager* m_parent;
};