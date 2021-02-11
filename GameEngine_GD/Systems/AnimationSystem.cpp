#include "AnimationSystem.h"
#include "../Interface/States.h"


AnimationSystem::AnimationSystem(void) {}

AnimationSystem::~AnimationSystem(void) {}

void AnimationSystem::tick(ECS::World* world, float deltaTime)
{
	if (States::GetPausedState() == false)
	{
		world->each<Animator, Sprite2D>(
			[&](ECS::Entity* entity,
				ECS::ComponentHandle<Animator> animator,
				ECS::ComponentHandle<Sprite2D> sprite
				) -> void
		{
			animator->currentTime += deltaTime;

			if (animator->currentTime >= animator->nextFrameTime)
			{
				animator->currentTime = 0;
				animator->currentColumn = (animator->currentColumn + 1) % animator->totalColumns;
			}

			// Size of 1 frame per sprite
			sprite->picture.setTextureRect(sf::IntRect(
				animator->currentColumn * animator->spriteWidth,	// left/right sides
				animator->currentRow * animator->spriteHeight,		// top/bottom sides
				animator->spriteWidth,
				animator->spriteHeight));
		});
	}
}
