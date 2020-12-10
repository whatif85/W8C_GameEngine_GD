#include "RenderingSystem.h"


RenderingSystem::RenderingSystem(void)
{
}

RenderingSystem::~RenderingSystem(void)
{
}

void RenderingSystem::tick(ECS::World* world, float deltaTime)
{
	// Clear before drawing all textures
	Engine::GetInstance().window->clear();

	world->each<Transform, Sprite2D>(
		[&](ECS::Entity* entity,
			ECS::ComponentHandle<Transform> transform,
			ECS::ComponentHandle<Sprite2D> sprite
			) -> void
	{
		// Add a texture to the unordered map
		if (textureMap.count(sprite->texturePath) < 1)
		{
			textureMap[sprite->texturePath] = LoadTexture(sprite->texturePath);
		}

		// If no texture is found, add it
		if (sprite->picture.getTexture() == nullptr)
		{
			sprite->picture.setTexture(*textureMap[sprite->texturePath]);
			sprite->width = sprite->picture.getGlobalBounds().width;
			sprite->height = sprite->picture.getGlobalBounds().height;
		}

		// Update and draw to the screen
		sprite->picture.setPosition(transform->xPos, transform->yPos);
		Engine::GetInstance().window->draw(sprite->picture);
	});

	// Display updates
	Engine::GetInstance().window->display();
}

sf::Texture* RenderingSystem::LoadTexture(std::string textureFile)
{
	sf::Texture* _textureRef = new sf::Texture();

	// If the texture is not found
	if (_textureRef->loadFromFile(textureFile) == false)
	{
		std::cerr << "ERROR: Unable to load image " << textureFile << ".\nIs this image in the correct directory?" << std::endl;

		//fprintf(stderr, "ERROR: Unable to load file \'%s\'\n", textureFile.data());

		//system("Pause");
		int _char = std::getchar();

		exit(EXIT_FAILURE);
	}

	return _textureRef;
}
