#include "Engine.h"


int main(int argc, char* args[])
{
	// Declare and get instance of singleton
	Engine& gameEngine = Engine::GetInstance();

	sf::RenderWindow window(sf::VideoMode(800, 600), "My Game Engine");

	// Create world for attaching entities and systems to
	gameEngine.world = ECS::World::createWorld();

	// Create our first entity
	ECS::Entity* stickFigure;

	// Add systems to the engine
	gameEngine.AddSystem(new RenderingSystem());

	// Create and assign 250 entities to the world
	for (int x = 0; x < 25; x++)
	{
		for (int y = 0; y < 10; y++)
		{
			// Add entity to the world
			stickFigure = gameEngine.world->create();

			// Assign components to entities after creation
			stickFigure->assign<Transform>(x * 25, y * 32);
			stickFigure->assign<Sprite2D>("../Debug/Pics/hero.png");

			std::cout << stickFigure->getEntityId() << " is the entity ID." << std::endl;
		}
	}

	// Pass window reference to the engine and start
	gameEngine.Start(&window);

	return 0;
}