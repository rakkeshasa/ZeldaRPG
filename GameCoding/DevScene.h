#pragma once
#include "Scene.h"

class Actor;
class Player;
class GameObject;
class UI;

struct PQNode
{
	PQNode(int32 cost, Vec2Int pos) : cost(cost), pos(pos) { }

	bool operator<(const PQNode& other) const { return cost < other.cost; }
	bool operator>(const PQNode& other) const { return cost > other.cost; }

	int32 cost;
	Vec2Int pos;
};

class DevScene : public Scene
{
	using Super = Scene;
public:
	DevScene();
	virtual ~DevScene();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;

	virtual void AddActor(Actor* actor) override;
	virtual void RemoveActor(Actor* actor) override;

	void LoadMap();
	void LoadPlayer();
	void LoadMonster();
	void LoadProjectile();
	void LoadEffect();
	void LoadTilemap();

	template<typename T>
	T* SpawnObject(Vec2Int pos)
	{
		// Type-Trait
		auto isGameObject = std::is_convertible_v<T*, GameObject*>;
		assert(isGameObject);

		T* ret = new T();
		ret->SetCellPos(pos, true);
		AddActor(ret);

		ret->BeginPlay();

		return ret;
	}

	template<typename T>
	T* SpawnObjectAtRandomPos()
	{
		Vec2Int randPos = GetRandomEmptyCellPos();
		return SpawnObject<T>(randPos);
	}

	Player* FindClosestPlayer(Vec2Int cellPos);
	bool FindPath(Vec2Int src, Vec2Int dest, vector<Vec2Int>& path, int32 maxDepth = 10);

	bool CanGo(Vec2Int cellPos);
	Vec2 ConvertPos(Vec2Int cellPos);
	Vec2Int GetRandomEmptyCellPos();

private:
	void TickMonsterSpawn();

private:
	const int32 DESIRED_MONSTER_COUNT = 20;
	int32 _monsterCount = 0;

	class TilemapActor* _tilemapActor = nullptr;
};

