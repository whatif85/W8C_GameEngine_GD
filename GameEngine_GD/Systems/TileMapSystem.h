#pragma once

#include "../Engine.h"
#include "../Interface/Button.h"


class TileMapSystem : public ECS::EntitySystem
{
public:
	TileMapSystem();
	~TileMapSystem();

	void tick(ECS::World* world, float deltaTime);
	void AddTileOnClick(bool bHasCollision, ECS::ComponentHandle<struct TileMap> tileMap);
};