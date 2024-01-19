#include "pch.h"
#include "Monster.h"
#include "ResourceManager.h"
#include "DevScene.h"
#include "SceneManager.h"
#include "Player.h"
#include "TimeManager.h"
#include "HitEffect.h"

Monster::Monster()
{
	_flipbookMove[DIR_UP] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_SnakeUp");
	_flipbookMove[DIR_DOWN] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_SnakeDown");
	_flipbookMove[DIR_LEFT] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_SnakeLeft");
	_flipbookMove[DIR_RIGHT] = GET_SINGLE(ResourceManager)->GetFlipbook(L"FB_SnakeRight");

	_stat.hp = 50;
	_stat.maxHp = 50;
	_stat.attack = 10;
	_stat.defence = 0;
}

Monster::~Monster()
{

}

void Monster::BeginPlay()
{
	Super::BeginPlay();

	SetState(ObjectState::Move); // ù state�� Idle�̹Ƿ� move�� �ٲ���
	SetState(ObjectState::Idle);
}

void Monster::Tick()
{
	Super::Tick();

}

void Monster::Render(HDC hdc)
{
	Super::Render(hdc);

}

void Monster::TickIdle()
{
	DevScene* scene = dynamic_cast<DevScene*>(GET_SINGLE(SceneManager)->GetCurrentScene());
	if (scene == nullptr)
		return;

	// Find Player
	if (_target == nullptr)
		_target = scene->FindClosestPlayer(GetCellPos());

	if (_target)
	{
		Vec2Int dir = _target->GetCellPos() - GetCellPos();
		int32 dist = abs(dir.x) + abs(dir.y);
		if (dist == 1)
		{
			// attack
			SetDir(GetLookAtDir(_target->GetCellPos()));
			SetState(ObjectState::Skill);
			_waitSeconds = 0.5f; // ���� ���� �ð�
		}
		else
		{
			vector<Vec2Int> path;
			if (scene->FindPath(GetCellPos(), _target->GetCellPos(), path))
			{
				if (path.size() > 1)
				{
					Vec2Int nextPos = path[1];
					if (scene->CanGo(nextPos))
					{
						SetCellPos(nextPos);
						SetState(ObjectState::Move);
					}
				}
				else
					SetCellPos(path[0]);
			}
		}
	}
}

void Monster::TickMove()
{
	float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();

	Vec2 dir = (_destPos - _pos);
	if (dir.Length() < 5.f)
	{
		SetState(ObjectState::Idle);
		_pos = _destPos;
	}
	else
	{
		bool horizontal = abs(dir.x) > abs(dir.y);
		if (horizontal)
			SetDir(dir.x < 0 ? DIR_LEFT : DIR_RIGHT);
		else
			SetDir(dir.y < 0 ? DIR_UP : DIR_DOWN);

		switch (_dir)
		{
		case DIR_UP:
			_pos.y -= 50 * deltaTime;
			break;
		case DIR_DOWN:
			_pos.y += 50 * deltaTime;
			break;
		case DIR_LEFT:
			_pos.x -= 50 * deltaTime;
			break;
		case DIR_RIGHT:
			_pos.x += 50 * deltaTime;
			break;
		}
	}
}

void Monster::TickSkill()
{
	if (_flipbook == nullptr)
		return;

	if (_waitSeconds > 0)
	{
		float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();
		_waitSeconds = max(0, _waitSeconds - deltaTime);
		return;
	}

	DevScene* scene = dynamic_cast<DevScene*>(GET_SINGLE(SceneManager)->GetCurrentScene());
	if (scene == nullptr)
		return;

	Creature* creature = scene->GetCreatureAt(GetFrontCellPos());
	if (creature)
	{
		scene->SpawnObject<HitEffect>(GetFrontCellPos()); // ����Ʈ
		// damage
		creature->OnDamaged(this);
	}

	SetState(ObjectState::Idle);
}

void Monster::UpdateAnimation()
{
	SetFlipbook(_flipbookMove[_dir]);
}

void Monster::OnDamaged(GameObject* attacker)
{
	Super::OnDamaged(attacker);

	if (_state == ObjectState::Idle)
	{
		SetState(ObjectState::Move);
	}
}