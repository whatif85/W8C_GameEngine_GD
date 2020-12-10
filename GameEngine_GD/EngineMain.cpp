#include "Engine.h"


int main(int argc, char* args[])
{
	// Declare and get instance of singleton
	Engine& gameEngine = Engine::GetInstance();

	sf::RenderWindow window(sf::VideoMode(800, 600), "My Game Engine");

	// Create world for attaching entities and systems to
	gameEngine.world = ECS::World::createWorld();

	// Create our entities
	ECS::Entity* background;
	ECS::Entity* stickFigure;
	ECS::Entity* tux;

	// Add systems to the engine
	gameEngine.AddSystem(new RenderingSystem());
	gameEngine.AddSystem(new AnimationSystem());

	/*
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
			//printf("Entity ID is %lu\n", stickFigure->getEntityId()); // long unsigned
		}
	}
	*/

	// Create 3 entities to the world
	background = gameEngine.world->create();
	stickFigure = gameEngine.world->create();
	tux = gameEngine.world->create();

	// Assign components to entities after creation
	background->assign<Transform>(0, 0);
	background->assign<Sprite2D>("../Debug/Pics/bg.jpg");

	stickFigure->assign<Transform>(300, 300);
	stickFigure->assign<Sprite2D>("../Debug/Pics/herosheet.png");
	stickFigure->assign<Animator>(32, 32, 200.0f, 4, 1);

	tux->assign<Transform>(200, 200);
	tux->assign<Sprite2D>("../Debug/Pics/tux_from_linux.png");
	tux->assign<Animator>(56, 72, 2000.0f, 3, 9);
	tux->get<Animator>()->currentRow = 0; // idle row

	std::cout << background->getEntityId() << " is the entity ID." << std::endl;
	std::cout << stickFigure->getEntityId() << " is the entity ID." << std::endl;
	std::cout << tux->getEntityId() << " is the entity ID." << std::endl;
	//printf("Entity ID is %lu\n", stickFigure->getEntityId()); // long unsigned

	// Pass window reference to the engine and start
	gameEngine.Start(&window);

	return 0;
}