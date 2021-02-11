#include "Engine.h"


Engine& Engine::GetInstance(void)
{
	static Engine _instance;
	return _instance;
}

void Engine::Start(sf::RenderWindow* win)
{
	bQuit = false;
	window = win;

	pauseMenu = PauseMenu(this->window);

	mainCam = MainCamera(sf::Vector2f(
		this->window->getSize().x / 2,
		this->window->getSize().y / 2));

	while (window->isOpen() == true)
	{
		Update();
	}
}

void Engine::AddSystem(ECS::EntitySystem* newSys)
{
	world->registerSystem(newSys);
	world->enableSystem(newSys);
}

Engine::Engine(void) {}

Engine::~Engine(void) {}

void Engine::Update()
{
	sf::Event _event;

	while (window->pollEvent(_event) == true)
	{
		// "Close requested" event
		if (_event.type == sf::Event::Closed)
		{
			window->close();
		}

		pauseMenu.Update(_event, 10.0f, window);
	}

	world->tick(10.0f);

	mainCam.Update(world, 10.0f, window);

	if (States::GetPausedState() == true)
	{
		GameInactiveStateActions();
	}
}

void Engine::GameInactiveStateActions()
{
	pauseMenu.Render(window, 10.0f, mainCam.cameraView.getCenter());
}
