#include "pch.h"
#include "Game.h"
#include "TimeManager.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "SoundManager.h"

Game::Game()
{
	
}

Game::~Game()
{
	GET_SINGLE(SceneManager)->Clear();
	GET_SINGLE(ResourceManager)->Clear();

	_CrtDumpMemoryLeaks();
}

void Game::Init(HWND hwnd)
{
	_hwnd = hwnd; // ������ ��ȣ
	_hdc = ::GetDC(hwnd); // ��ȭ�� ��ȣ

	::GetClientRect(hwnd, &_rect);

	// �� �ٸ� ��ȭ�� ����
	_hdcBack = ::CreateCompatibleDC(_hdc); // _hdc�� ȣȯ�Ǵ� dc
	_bmpBack = ::CreateCompatibleBitmap(_hdc, _rect.right, _rect.bottom); // _hdc�� ȣȯ�Ǵ� ��Ʈ��
	HBITMAP prev = (HBITMAP)::SelectObject(_hdcBack, _bmpBack); // DC�� BMP�� ����
	::DeleteObject(prev); // ���� ��ȭ���� ����

	GET_SINGLE(TimeManager)->Init();
	GET_SINGLE(InputManager)->Init(hwnd);
	GET_SINGLE(SceneManager)->Init();
	GET_SINGLE(ResourceManager)->Init(hwnd, fs::path(L"D:\\Program Files\\Rookiss\\GameCoding\\Resources"));
	GET_SINGLE(SoundManager)->Init(hwnd);
	// ù ���� Scene
	GET_SINGLE(SceneManager)->ChangeScene(SceneType::DevScene);
}

void Game::Update()
{
	GET_SINGLE(TimeManager)->Update();
	GET_SINGLE(InputManager)->Update();
	GET_SINGLE(SceneManager)->Update();
}

void Game::Render()
{
	GET_SINGLE(SceneManager)->Render(_hdcBack);

	uint32 fps = GET_SINGLE(TimeManager)->GetFps();
	float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();

	{
		POINT mousePos = GET_SINGLE(InputManager)->GetMousePos();
		wstring str = std::format(L"Mouse({0}, {1})", mousePos.x, mousePos.y);
		::TextOut(_hdcBack, 20, 10, str.c_str(), static_cast<int32>(str.size()));
	}
	
	{
		wstring str = std::format(L"FPS({0}), DT({1} ms)", fps, static_cast<int32>(deltaTime * 1000));
		::TextOut(_hdcBack, 650, 10, str.c_str(), static_cast<int32>(str.size()));
	}
	
	// ���� ���۸�
	// _hdcBack�� _hdc�� �׷��ش�
	::BitBlt(_hdc, 0, 0, _rect.right, _rect.bottom, _hdcBack, 0, 0 , SRCCOPY);
	// ����۸� �о��ش�
	::PatBlt(_hdcBack, 0, 0, _rect.right, _rect.bottom, WHITENESS);
}
