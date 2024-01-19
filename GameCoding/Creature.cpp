#include "pch.h"
#include "Creature.h"
#include "SceneManager.h"
#include "DevScene.h"
#include "Projectile.h"

Creature::Creature()
{

}

Creature::~Creature()
{

}

void Creature::BeginPlay()
{
	Super::BeginPlay();

}

void Creature::Tick()
{
	Super::Tick();

}

void Creature::Render(HDC hdc)
{
	Super::Render(hdc);

}

void Creature::OnDamaged(GameObject* attacker)
{
	if (attacker == nullptr)
		return;

	if (dynamic_cast<Projectile*>(attacker))
	{
		Projectile* p_attacker = dynamic_cast<Projectile*>(attacker);
		Creature* owner = p_attacker->GetCreature();

		Stat& attackerStat = owner->GetStat();
		Stat& stat = GetStat();

		int32 damage = attackerStat.attack - stat.defence;
		if (damage <= 0)
			return;

		stat.hp = max(0, stat.hp - damage);

		if (stat.hp == 0)
		{
			Scene* scene = GET_SINGLE(SceneManager)->GetCurrentScene();
			if (scene)
			{
				scene->RemoveActor(this);
			}
		}
	}
	else if (dynamic_cast<Creature*>(attacker))
	{
		Creature* c_attacker = dynamic_cast<Creature*>(attacker);
		Stat& attackerStat = c_attacker->GetStat();
		Stat& stat = GetStat();

		int32 damage = attackerStat.attack - stat.defence;
		if (damage <= 0)
			return;

		stat.hp = max(0, stat.hp - damage);

		if (stat.hp == 0)
		{
			Scene* scene = GET_SINGLE(SceneManager)->GetCurrentScene();
			if (scene)
			{
				scene->RemoveActor(this);
			}
		}
	}
	else
		return;
}
