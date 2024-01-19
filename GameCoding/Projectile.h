#pragma once
#include "GameObject.h"

class Creature;

class Projectile : public GameObject
{
	using Super = GameObject;

public:
	Projectile();
	virtual ~Projectile() override;

	virtual void BeginPlay() override;
	virtual void Tick() override;
	virtual void Render(HDC hdc) override;

	Creature* GetCreature() { return _owner; }
	void SetCreature(Creature* creature) { _owner = creature; }

protected:
	virtual void TickIdle() override {}
	virtual void TickMove() override {}
	virtual void TickSkill() override {}
	virtual void UpdateAnimation() {}

private:
	Creature* _owner = nullptr;
};

