#pragma once

#ifndef SYSTEM_RENDER_H
#define SYSTEM_RENDER_H 1

#include "../ECS.h"
#include "../Engine.h"
#include "../Components.h"


class RenderingSystem
	: public ECS::EntitySystem
{
public:
	RenderingSystem(void);
	~RenderingSystem(void);

	void tick(ECS::World* world, float deltaTime) override;

private:
	std::unordered_map<std::string, sf::Texture*> textureMap;

	sf::Texture* LoadTexture(std::string textureFile);
};

#endif