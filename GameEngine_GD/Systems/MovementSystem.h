#pragma once

#include "../ECS.h"
#include "../Engine.h"
#include "../Components.h"


class MovementSystem
	: public ECS::EntitySystem
{
public:
	MovementSystem();
	~MovementSystem();

	void tick(ECS::World* world, float deltaTime) override;
};

