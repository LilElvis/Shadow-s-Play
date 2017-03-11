/*
Brought to you by the Game Programming club!
*/

#include "Input.h"

#include <iostream>

namespace ENG
{

	// Set all of the flags to 0
	char Input::keyPressedFlag[256] = { 0 };
	char Input::keyHandledFlag[256] = { 0 };
	char Input::keyReleasedFlag[256] = { 0 };
	char Input::keyWasPressed[256] = { 0 };

	bool Input::GetKey(KeyCode::Keys iKey)
	{
		// Check if the key is pressed
		bool keyPress = (GetAsyncKeyState(iKey) >> 15) & 1;

		// If it's pressed, set the pressed flag to true
		if (keyPress)
		{
			keyWasPressed[iKey] = 1;
			//std::cout << ("key pressed\n");
		}
		else
		{
			//std::cout << ("NOPE\n");
		}

		// Return whether it's pressed or not
		return keyPress;
	}

	bool Input::GetKeyDown(KeyCode::Keys iKey)
	{
		// If the key was handled already, we know it's true!
		if (keyHandledFlag[iKey] == 1)
		{
			return true;
		}
		// If the key is down, and it wasn't previously held down, we know it's a press
		if (GetKey(iKey) && keyPressedFlag[iKey] == 0)
		{
			// We set the key to be handled
			keyHandledFlag[iKey] = 1;
			// We set the key to have been pressed
			keyPressedFlag[iKey] = 1;
			return true;
		}
		// If the key isn't being held down, we can set the pressed flag to false
		if (!GetKey(iKey))
		{
			keyPressedFlag[iKey] = 0;
		}
		// and therefore return false
		return false;
	}

	bool Input::GetKeyUp(KeyCode::Keys iKey)
	{
		// If the key has been released already, return true
		if (keyReleasedFlag[iKey] == 1)
			return true;

		// If the key isn't being pressed, but was pressed previously, it's been released
		if (!GetKey(iKey) && keyWasPressed[iKey] == 1)
		{
			// And we save that information in a flag
			keyReleasedFlag[iKey] = 1;
			// We return true if it's been released
			return true;
		}
		// Otherwise, we return false
		return false;
	}

	void Input::ResetKeys()
	{
		// For each individual key
		for (int i = 0; i < 256; i++)
		{
			// If it's not being pressed, we set the flag to 0
			if (((GetAsyncKeyState(i) >> 15) & 1) == 0)
			{
				keyWasPressed[i] = 0;
			}

			// We set the released flag and handled flag to zero every frame
			keyReleasedFlag[i] = 0;
			keyHandledFlag[i] = 0;
		}
	}

}