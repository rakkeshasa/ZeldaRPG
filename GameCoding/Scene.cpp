#include "pch.h"
#include "Scene.h"
#include "Actor.h"
#include "Creature.h"
#include "UI.h"
#include "TimeManager.h"
#include "SceneManager.h"
#include "CollisionManager.h"

Scene::Scene()
{
}

Scene::~Scene()
{
}

void Scene::Init()
{
	{
		for (const vector<Actor*>& actors : _actors)
			for (Actor* actor : actors)
				actor->BeginPlay();

		for (UI* ui : _uis)
			ui->BeginPlay();
	}
}

void Scene::Update()
{
	float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();

	GET_SINGLE(CollisionManager)->Update();

	// remove�� �� �����Ƿ� ���� ���
	for (const vector<Actor*> actors : _actors)
		for (Actor* actor : actors)
			actor->Tick();

	for (UI* ui : _uis)
		ui->Tick();
}

void Scene::Render(HDC hdc)
{
	vector<Actor*>& actors = _actors[LAYER_OBJECT];
	std::sort(actors.begin(), actors.end(), [=](Actor* a, Actor* b)
		{
			return a->GetPos().y < b->GetPos().y;
		});

	for (const vector<Actor*>& actors : _actors)
		for (Actor* actor : actors)
			actor->Render(hdc);

	for (UI* ui : _uis)
		ui->Render(hdc);
}

void Scene::AddActor(Actor* actor)
{
	if (actor == nullptr)
		return;

	_actors[actor->GetLayer()].push_back(actor);
}

void Scene::RemoveActor(Actor* actor)
{
	if (actor == nullptr)
		return;

	vector<Actor*>& v = _actors[actor->GetLayer()];
	v.erase(std::remove(v.begin(), v.end(), actor), v.end());
}

Creature* Scene::GetCreatureAt(Vec2Int cellPos)
{
	for (Actor* actor : _actors[LAYER_OBJECT])
	{
		Creature* creature = dynamic_cast<Creature*>(actor);
		if (creature && creature->GetCellPos() == cellPos)
			return creature;
	}

	return nullptr;
}
