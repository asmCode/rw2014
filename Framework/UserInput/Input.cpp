#include "Input.h"
#include <Windows.h>

bool Input::m_pressed[KeyCode::Count];
bool Input::m_justPressed[KeyCode::Count];
bool Input::m_justReleased[KeyCode::Count];
int Input::m_keyMap[KeyCode::Count];

bool Input::m_constructorTrigger = Input::Constructor();

bool Input::Constructor()
{
	Input::m_keyMap[KeyCode_Left] = VK_LEFT;
	Input::m_keyMap[KeyCode_Right] = VK_RIGHT;
	Input::m_keyMap[KeyCode_Up] = VK_UP;
	Input::m_keyMap[KeyCode_Down] = VK_DOWN;

	Input::m_keyMap[KeyCode_A] = 'A';
	Input::m_keyMap[KeyCode_B] = 'B';
	Input::m_keyMap[KeyCode_C] = 'C';
	Input::m_keyMap[KeyCode_D] = 'D';
	Input::m_keyMap[KeyCode_E] = 'E';
	Input::m_keyMap[KeyCode_F] = 'F';
	Input::m_keyMap[KeyCode_G] = 'G';
	Input::m_keyMap[KeyCode_H] = 'H';
	Input::m_keyMap[KeyCode_I] = 'I';
	Input::m_keyMap[KeyCode_J] = 'J';
	Input::m_keyMap[KeyCode_K] = 'K';
	Input::m_keyMap[KeyCode_L] = 'L';
	Input::m_keyMap[KeyCode_M] = 'M';
	Input::m_keyMap[KeyCode_N] = 'N';
	Input::m_keyMap[KeyCode_O] = 'O';
	Input::m_keyMap[KeyCode_P] = 'P';
	Input::m_keyMap[KeyCode_Q] = 'Q';
	Input::m_keyMap[KeyCode_R] = 'R';
	Input::m_keyMap[KeyCode_S] = 'S';
	Input::m_keyMap[KeyCode_T] = 'T';
	Input::m_keyMap[KeyCode_U] = 'U';
	Input::m_keyMap[KeyCode_V] = 'V';
	Input::m_keyMap[KeyCode_W] = 'W';
	Input::m_keyMap[KeyCode_X] = 'X';
	Input::m_keyMap[KeyCode_Y] = 'Y';
	Input::m_keyMap[KeyCode_Z] = 'Z';

	Input::m_keyMap[KeyCode_Num0] = '0';
	Input::m_keyMap[KeyCode_Num1] = '1';
	Input::m_keyMap[KeyCode_Num2] = '2';
	Input::m_keyMap[KeyCode_Num3] = '3';
	Input::m_keyMap[KeyCode_Num4] = '4';
	Input::m_keyMap[KeyCode_Num5] = '5';
	Input::m_keyMap[KeyCode_Num6] = '6';
	Input::m_keyMap[KeyCode_Num7] = '7';
	Input::m_keyMap[KeyCode_Num8] = '8';
	Input::m_keyMap[KeyCode_Num9] = '9';

	Input::m_keyMap[KeyCode_Escape] = VK_ESCAPE;
	Input::m_keyMap[KeyCode_LShift] = VK_LSHIFT;
	Input::m_keyMap[KeyCode_LCtrl] = VK_LCONTROL;

	return true;
}

void Input::UpdateState()
{
	short keyState;
	short pressedFlag = (short)0x8000;

	for (unsigned char i = 0; i < static_cast<unsigned char>(KeyCode::Count); i++)
	{
		keyState = GetAsyncKeyState(m_keyMap[i]);

		if ((keyState & pressedFlag) != 0)
		{
			m_justPressed[i] = !m_pressed[i];
			
			m_pressed[i] = true;
		}
		else
		{			
			m_justReleased[i] = m_pressed[i];

			m_pressed[i] = false;
		}
	}
}

bool Input::GetKey(KeyCode keyCode)
{
	return m_pressed[keyCode];
}

bool Input::GetKeyDown(KeyCode keyCode)
{
	return m_justPressed[keyCode];
}

bool Input::GetKeyUp(KeyCode keyCode)
{
	return m_justReleased[keyCode];
}

