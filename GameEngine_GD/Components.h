#pragma once

#include <iostream>

#include "ECS.h"
#include "Engine.h"


struct Transform
{
public:
	ECS_DECLARE_TYPE;

	float xPos, yPos;
	float rotation;
	float xSpeed, ySpeed;
	float xSpeedMod, ySpeedMod;

	Transform(float newX, float newY, float newXSpeed = 0.0f, float newYSpeed = 0.0f)
		: xPos(newX), yPos(newY), xSpeedMod(newXSpeed), ySpeedMod(newYSpeed)
	{
		this->rotation = 0.0f;
		this->xSpeed = 0.0f;
		this->ySpeed = 0.0f;
	}

	void UpdateSpeed(float x, float y)
	{
		this->xSpeed = x;
		this->ySpeed = y;
	}

	void Move()
	{
		// halve the speed when moving diagonally
		if (xSpeed != 0 && ySpeed != 0)
		{
			xSpeed /= 2;
			ySpeed /= 2;
		}
		
		xPos += xSpeed;
		yPos += ySpeed;
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


struct InputController
{
public:
	ECS_DECLARE_TYPE;

	bool bInputActive;

	bool wKey, aKey, sKey, dKey;

	InputController()
	{
		bInputActive = true;

		wKey = false;
		aKey = false;
		sKey = false;
		dKey = false;
	}
};
ECS_DEFINE_TYPE(InputController);


struct BoxCollider
{
public:
	ECS_DECLARE_TYPE;

	int leftEdge, rightEdge, topEdge, bottomEdge;

	BoxCollider()
	{
		// set all structure's members to 0
		std::memset(this, '\0', sizeof(BoxCollider));
	}

	void Update(int xSide, int ySide, int width, int height)
	{
		leftEdge = xSide;
		rightEdge = xSide + width;
		topEdge = ySide;
		bottomEdge = ySide + height;
	}
};
ECS_DEFINE_TYPE(BoxCollider);


struct Camera
{
public:
	ECS_DECLARE_TYPE;

	sf::View cameraView;

	Camera(sf::Vector2f pivot)
	{
		cameraView.setCenter(pivot);
	}
};
ECS_DEFINE_TYPE(Camera);


struct Tag
{
public:
	ECS_DECLARE_TYPE;

	std::vector<std::string> tagNames;

	Tag() {}

	void AddTag(std::string tag)
	{
		tagNames.push_back(tag);
	}
};
ECS_DEFINE_TYPE(Tag);