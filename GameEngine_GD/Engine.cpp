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

	while (window->isOpen() == true)
	{
		Update();
	}
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
	}
}
