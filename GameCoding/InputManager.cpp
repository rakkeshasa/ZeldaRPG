#include "pch.h"
#include "InputManager.h"

void InputManager::Init(HWND hwnd)
{
	_hwnd = hwnd; // ���߿� �ʿ��� �� ���� ���� ����
	_states.resize(KEY_TYPE_COUNT, KeyState::None); // 256�� none����
}

void InputManager::Update()
{
	// ��� ���� �ܾ����(256��)
	BYTE asciiKeys[KEY_TYPE_COUNT] = {};
	if (::GetKeyboardState(asciiKeys) == false)
		return;

	// ���� üũ�ϱ�
	for (uint32 key = 0; key < KEY_TYPE_COUNT; key++)
	{
		// Ű�� ���� ������ true
		if (asciiKeys[key] & 0x80)
		{
			KeyState& state = _states[key];

			// ���� �����ӿ� Ű�� �����°�?
			// ���� �� down ������ ���� �� �� press
			if (state == KeyState::Press || state == KeyState::Down)
				state = KeyState::Press;
			else
				state = KeyState::Down;
		}
		else
		{
			// Ű���带 �ô�
			KeyState& state = _states[key];

			// ������ �ִٰ� �ô�
			if (state == KeyState::Press || state == KeyState::Down)
				state = KeyState::Up;
			else // �������� ���� �־���.
				state = KeyState::None;
		}

		// ���콺
		::GetCursorPos(&_mousePos); // Ŀ���� ��ġ
		::ScreenToClient(_hwnd, &_mousePos); // â ���������� Ŀ���� ��ġ
	}
}
