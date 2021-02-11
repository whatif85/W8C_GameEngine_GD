#include "..\GameEngine_GD\Engine.h"


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
	ECS::Entity* pistol;

	// Add systems to the engine
	gameEngine.AddSystem(new RenderingSystem());
	gameEngine.AddSystem(new AnimationSystem());
	gameEngine.AddSystem(new InputSystem(&window));
	gameEngine.AddSystem(new MovementSystem());
	gameEngine.AddSystem(new PhysicsSystem());

	// Create 3 entities to the world
	background = gameEngine.world->create();
	stickFigure = gameEngine.world->create();
	tux = gameEngine.world->create();
	pistol = gameEngine.world->create();

	// Assign components to entities after creation
	background->assign<Transform>(0, 0);
	background->assign<Sprite2D>("../Debug/Pics/bg.jpg");
	background->assign<Tag>();
	background->get<Tag>()->AddTag("Background");

	stickFigure->assign<Transform>(300, 300, 0.2f, 0.2f);
	stickFigure->assign<Sprite2D>("../Debug/Pics/herosheet.png");
	stickFigure->assign<Animator>(32, 32, 200.0f, 4, 0);
	stickFigure->get<Animator>()->currentRow = 0; // idle row
	stickFigure->assign<BoxCollider>();
	stickFigure->assign<Tag>();
	stickFigure->get<Tag>()->AddTag("NPC");

	tux->assign<Transform>(200, 200, 0.3f, 0.3f);
	tux->assign<Sprite2D>("../Debug/Pics/tux_from_linux.png");
	tux->assign<Animator>(56, 72, 2000.0f, 3, 9);
	tux->get<Animator>()->currentRow = 0; // idle row
	tux->assign<InputController>();
	tux->assign<BoxCollider>();
	tux->assign<Camera>(sf::Vector2f(
		window.getSize().x / 2,
		window.getSize().y / 2));
	tux->assign<Tag>();
	tux->get<Tag>()->AddTag("Player");

	pistol->assign<Transform>(300, 100);
	pistol->assign<Sprite2D>("../Debug/Pics/pistol.png");
	pistol->assign<BoxCollider>();
	pistol->assign<Tag>();
	pistol->get<Tag>()->AddTag("Object");

	std::cout << background->getEntityId() << " is the entity ID." << std::endl;
	std::cout << stickFigure->getEntityId() << " is the entity ID." << std::endl;
	std::cout << tux->getEntityId() << " is the entity ID." << std::endl;
	//printf("Entity ID is %lu\n", stickFigure->getEntityId()); // long unsigned

	//printf("X: %f\nY: %f\n", background->get<Transform>()->xPos, background->get<Transform>()->yPos);

	// Pass window reference to the engine and start
	gameEngine.Start(&window);

	return 0;
}