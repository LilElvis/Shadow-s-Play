#include "GameStates.h"

//ADDS STATE TO MANAGER
void GameStateManager::AddGameState(std::string a_string, GameState* a_state)
{
	m_states[a_string] = a_state;
	a_state->m_parent = this;
}

//REMOVES STATE FROM MANAGER
void GameStateManager::RemoveGameState(std::string a_string)
{
	//m_states[a_string]->Release();
	m_states[a_string] = nullptr;
}

//RUNS MAIN LOOP OF EACH STATE
void GameStateManager::Update()
{
	for (auto itr = m_states.begin(), itrEnd = m_states.end(); itr != itrEnd; itr++)
	{
		if (itr->second != nullptr && itr->second->m_paused == false)
		{
			itr->second->Update();
		}
	}
}

//RETURNS A STATE BY NAME
GameState* GameStateManager::GetGameState(std::string a_string)
{
	if (m_states[a_string] != NULL)
		return m_states[a_string];
	std::cout << "ERROR, NO GAMESTATE WITH NAME " << a_string;
}

///Game States
void GameState::SetPaused(bool a_paused)
{
	m_paused = a_paused;
}