#pragma once

class Game
{
public:
	Game();
	~Game();

public:
	void Init(HWND hwnd);
	void Update();
	void Render();

private:
	HWND _hwnd = {};
	HDC _hdc = {};

private:
	// 더블 버퍼링
	RECT _rect; // 현재 그리는 영역
	HDC _hdcBack = {}; // 백버퍼
	HBITMAP _bmpBack = {}; // 비트맵
};

