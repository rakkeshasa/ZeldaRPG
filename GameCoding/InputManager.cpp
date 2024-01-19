#include "pch.h"
#include "InputManager.h"

void InputManager::Init(HWND hwnd)
{
	_hwnd = hwnd; // 나중에 필요할 때 쓰기 위해 저장
	_states.resize(KEY_TYPE_COUNT, KeyState::None); // 256개 none상태
}

void InputManager::Update()
{
	// 모든 상태 긁어오기(256개)
	BYTE asciiKeys[KEY_TYPE_COUNT] = {};
	if (::GetKeyboardState(asciiKeys) == false)
		return;

	// 상태 체크하기
	for (uint32 key = 0; key < KEY_TYPE_COUNT; key++)
	{
		// 키가 눌려 있으면 true
		if (asciiKeys[key] & 0x80)
		{
			KeyState& state = _states[key];

			// 이전 프레임에 키를 눌렀는가?
			// 누를 때 down 눌림이 유지 될 때 press
			if (state == KeyState::Press || state == KeyState::Down)
				state = KeyState::Press;
			else
				state = KeyState::Down;
		}
		else
		{
			// 키보드를 뗐다
			KeyState& state = _states[key];

			// 누르고 있다가 뗐다
			if (state == KeyState::Press || state == KeyState::Down)
				state = KeyState::Up;
			else // 원래부터 떼고 있었다.
				state = KeyState::None;
		}

		// 마우스
		::GetCursorPos(&_mousePos); // 커서의 위치
		::ScreenToClient(_hwnd, &_mousePos); // 창 기준으로한 커서의 위치
	}
}
