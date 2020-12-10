#pragma once

#include <iostream>

#include "ECS.h"
#include "Engine.h"


struct Transform
{
public:
	ECS_DECLARE_TYPE;

	float xPos, yPos, rotation;

	Transform(float newX, float newY)
		: xPos(newX), yPos(newY)
	{
		this->rotation = 0.0f;
	}
};
ECS_DEFINE_TYPE(Transform);


struct Sprite2D
{
public:
	ECS_DECLARE_TYPE;

	sf::Sprite picture;
	std::string texturePath;
	int width, height;

	Sprite2D(std::string name)
	{
		texturePath = name;
	}
};
ECS_DEFINE_TYPE(Sprite2D);


struct Animator
{
public:
	ECS_DECLARE_TYPE;

	int spriteWidth, spriteHeight;
	int currentColumn, currentRow;
	int totalColumns, totalRows;

	float currentTime, nextFrameTime;

	bool bFacingRight;

	Animator(int newWidth, int newHeight, float timeBetweenFrames, int newColumns, int newRows)
	{
		this->spriteWidth = newWidth;
		this->spriteHeight = newHeight;

		currentColumn = 0;
		currentRow = 0;

		this->totalColumns = newColumns;
		this->totalRows = newRows;

		this->nextFrameTime = timeBetweenFrames;

		currentTime = 0;

		bFacingRight = true;
	}
};
ECS_DEFINE_TYPE(Animator);