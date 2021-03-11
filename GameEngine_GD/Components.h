#pragma once

#include <iostream>
#include <fstream>

#include "ECS.h"
#include "Engine.h"
#include "Interface/Tile.h"


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


struct TileMap
{
public:
	ECS_DECLARE_TYPE;

	float gridSizeF; // NOTE: Change this value to increase the size of the whole grid for more tile insertion

	uint32_t gridSizeU; // still unsigned int
	uint32_t layers; // The layer value used for rendering images in a certain order (see constructor)

	sf::Vector2u maxSize; // Vector to set the size of each tile
	sf::RectangleShape collisionBox; // The box outline of the tile which is rendered in RenderingSystem.cpp

	/**
		Map structure:
		1. Column of vectors
		2. Row of vectors
		3. Collection of tile pointers
	*/
	std::vector <std::vector<std::vector<Tile*>>> map; // vector in std is an "ArrayList" (not to be confused with math vector)

	TileMap()
	{
		this->gridSizeF = 50.0f;

		// return a value of new type, convert float to unsigned int
		//this->gridSizeU = static_cast<unsigned>(this->gridSizeF);
		this->gridSizeU = static_cast<uint32_t>(this->gridSizeF);

		// number of tiles (size of tiles given by gridSize)
		this->maxSize.x = 20;
		this->maxSize.y = 20;

		this->layers = 1;

		this->map.resize(this->maxSize.x);

		for (size_t _x = 0; _x < this->maxSize.x; _x++)
		{
			this->map.push_back(std::vector<std::vector<Tile*>>());

			for (size_t _y = 0; _y < this->maxSize.y; _y++)
			{
				this->map.at(_x).resize(this->maxSize.y);
				this->map[_x].push_back(std::vector<Tile*>());

				for (size_t _z = 0; _z < this->layers; _z++)
				{
					this->map.at(_x).at(_y).resize(this->layers);
					this->map[_x][_y].push_back(nullptr);
				}
			}
		}

		this->collisionBox.setSize(sf::Vector2f(this->gridSizeF, this->gridSizeF));
		this->collisionBox.setFillColor(sf::Color(0, 255, 0, 50));
		this->collisionBox.setOutlineColor(sf::Color::Green);
		this->collisionBox.setOutlineThickness(-1.0f);
	}

	~TileMap()
	{
		Clear();
	}

	void AddTile(const int x, const int y, const int z, bool bHasCollision)
	{
		if (x < this->maxSize.x && x >= 0 &&
			y < this->maxSize.y && y >= 0 &&
			z < this->layers && z >= 0)
		{
			if (this->map[x][y][z] == nullptr) // if empty
			{

				this->map[x][y][z] = new Tile(x, y, this->gridSizeF, bHasCollision);
				std::cout << "ADDED" << std::endl;
			}
		}
	}

	void Clear()
	{
		for (size_t _x = 0; _x < this->maxSize.x; _x++)
		{
			for (size_t _y = 0; _y < this->maxSize.y; _y++)
			{
				for (size_t _z = 0; _z < this->layers; _z++)
				{
					delete this->map[_x][_y][_z];
					//this->map[_x][_y][_z] = nullptr;
				}
				this->map[_x][_y].clear();
			}
			this->map[_x].clear();
		}
		this->map.clear();
	}

	//void SaveTileMap(std::string filePath)
	//{
	//	FILE* _saveFile;

	//	//std::ofstream _saveFile;
	//	//_saveFile.open(filePath);

	//	// write binary 
	//	// TODO: fopen_s() instead?
	//	_saveFile = fopen(filePath.data(), "wb");

	//	if (_saveFile == nullptr)
	//	{
	//		return;
	//	}

	//	/**
	//	* Write the details of the grid into the file
	//	* Functions like fprintf have a variable-length argument list,
	//	* which means that they don't have a fixed number of arguments
	//	* In here, the arguments depend on the second argument,
	//	* which is a format string, or a string that contains special specifiers
	//	* to say how to represent the corresponding argument
	//	* %d: decimal (double)
	//	* %f: float
	//	* %s: const char*(std::string::data())
	//	* %zu: size_t
	//	*/
	//	fprintf(_saveFile, "%d %d\n%f\n%d\n;", this->maxSize.x, this->maxSize.y, this->gridSizeF, this->layers);

	//	//if (_saveFile.is_open() == true)
	//	//{
	//		//_saveFile << maxSize.x << " " << maxSize.y << "\n"
	//		//	<< gridSizeF << "\n"
	//		//	<< layers << "\n";

	//	for (size_t _x = 0; _x < this->maxSize.x; _x++)
	//	{
	//		for (size_t _y = 0; _y < this->maxSize.y; _y++)
	//		{
	//			for (size_t _z = 0; _z < this->layers; _z++)
	//			{
	//				if (this->map[_x][_y][_z] != nullptr)
	//				{
	//					//_saveFile << _x << " " << _y << " " << _z << " " << this->map[_x][_y][_z]->ToString() << " " << "\n";
	//					fprintf(stderr, "Error: failed to save file \"%s\"\n", filePath.data());
	//					
	//					// size_t unsigned long
	//					fprintf(_saveFile, "%zu %zu %zu %s\n;", _x, _y, _z, this->map[_x][_y][_z]->ToString().data());
	//				}
	//			}
	//		}
	//	}
	//	//}
	//	//else
	//	//{
	//	//	std::cout << "Error: TileMap could not save to file " << filePath << "\n";

	//		
	//	//}

	//	//_saveFile.close();

	//	fclose(_saveFile);
	//}

	//void LoadTileMap(std::string filePath)
	//{
	//	FILE* _loadFile;	// file stream structure
	//	size_t _length;		// file length, used for allocating the buffer
	//	char* _buffer;		// pointer to memory allocated to store file contents

	//	sf::Vector2f _size;
	//	float _gridSizeF = 0.0f;
	//	uint32_t _layers = 0;
	//	size_t _x = 0;
	//	size_t _y = 0;
	//	size_t _z = 0;
	//	bool _bIsColliding = false;

	//	//std::ifstream _loadFile;
	//	//_loadFile.open(filePath);

	//	// read binary (open file in read-only mode
	//	_loadFile = fopen(filePath.data(), "rb");

	//	if (_loadFile == nullptr)
	//	{
	//		fprintf(stderr, "Error: failed to open file: %s\n", strerror(errno));
	//		return;
	//	}

	//	/* determine file length */
	//	fseek(_loadFile, 0, SEEK_END);	// go to end of file
	//	_length = ftell(_loadFile);		// get the current position in the file, which is also its length in this case
	//	fseek(_loadFile, 0, SEEK_SET);	// return to the start of the file

	//	/* allocate a buffer of length + 1 bytes to store the contents of the file */
	//	_buffer = (char*)calloc(_length + 1, sizeof(char));

	//	if (_buffer == nullptr)
	//	{
	//		/* Print an error message strerror gets a human-readable error message indicating why the function failed */
	//		fprintf(stderr, "Error: failed to allocate buffer: %s\n", strerror(errno));
	//		return;
	//	}

	//	// now we can read the file's contents into the buffer
	//	fread(_buffer, sizeof(char), _length, _loadFile);
	//	/**
	//	* 1. buffer to use
	//	* 2. size of the chunks to read
	//	* 3. how many chunks to read
	//	* 4. where to read from
	//	*/

	//	if (_buffer == nullptr)
	//	{
	//		fprintf(stderr, "Error: failed to read file: %s\n", strerror(errno));
	//		return;
	//	}

	//	// terminate the buffer (which is length + 1 bytes long)
	//	_buffer[_length] = 0;

	//	// close the file
	//	fclose(_loadFile);

	//	//if (_loadFile.is_open() == true)
	//	//{

	//	//_loadFile >> _size.x >> _size.y >> _gridSizeF >> _layers;

	//	// buffer[0] is the start of max_size.x as a string
	//	this->maxSize.x = atof(_buffer); // atof converts ASCII to a float
	//	this->maxSize.y = atof(&_buffer[sizeof(uint32_t)]); // this will be after the first float and the space after that
	//	this->gridSizeF = atof(&_buffer[(sizeof(uint32_t) * 2) + 2]); // this is after the second float and the new line
	//	this->layers = atof(&_buffer[(sizeof(uint32_t) * 2) + sizeof(float) + 3]); // and the last one

	//	this->gridSizeF = _gridSizeF;
	//	this->gridSizeU = static_cast<unsigned int>(this->gridSizeF);
	//	this->maxSize.x = _size.x; // number of tiles wide
	//	this->maxSize.y = _size.y; // number of tiles high
	//	this->layers = _layers;

	//	this->Clear();

	//	this->map.resize(this->maxSize.x, std::vector<std::vector<Tile*>>());

	//	for (size_t _x = 0; _x < this->maxSize.x; _x++)
	//	{
	//		for (size_t _y = 0; _y < this->maxSize.y; _y++)
	//		{
	//			this->map[_x].resize(this->maxSize.y, std::vector<Tile*>());

	//			for (size_t _z = 0; _z < this->layers; _z++)
	//			{
	//				this->map[_x][_y].resize(this->layers, NULL);
	//			}
	//		}
	//	}

	//	while (_loadFile >> _x >> _y >> _z >> _bIsColliding)
	//	{
	//		std::cout << _x << ", " << _y << ", " << _z << std::endl;

	//		this->map[_x][_y][_z] = new Tile(_x, _y, _gridSizeF, _bIsColliding);
	//	}

	//	//}
	//	//else
	//	//{
	//	//	std::cout << "Error: TileMap could not save " << filePath << "\n";
	//	//}

	//	//_loadFile.close();
	//	*/
	//}

	void SaveTileMap_O(std::string fileName)
	{
		std::ofstream _saveFile;
		_saveFile.open(fileName);

		if (_saveFile.is_open() == true)
		{
			_saveFile << maxSize.x << " " << maxSize.y << "\n"
				<< gridSizeF << "\n"
				<< layers << "\n";

			for (size_t x = 0; x < this->maxSize.x; x++)
			{
				for (size_t y = 0; y < this->maxSize.y; y++)
				{
					for (size_t z = 0; z < this->layers; z++)
					{
						if (this->map[x][y][z] != nullptr)
						{
							_saveFile << x << " " << y << " " << z << " " << this->map[x][y][z]->ToString() << " " << "\n";
						}
					}
				}
			}
		}
		else
		{
			std::cout << "Error: TileMap could not be saved to file " << fileName << "\n";
		}

		_saveFile.close();
	}

	void LoadTileMap_O(std::string fileName)
	{
		std::ifstream _loadFile;
		_loadFile.open(fileName);

		if (_loadFile.is_open() == true)
		{
			//sf::Vector2f _size;
			//float _gridSizeF = 0;
			//uint32_t _layers = 0;
			//uint32_t _x = 0;
			//uint32_t _y = 0;
			//uint32_t _z = 0;
			bool _bColliding = false;

			_loadFile >> maxSize.x >> maxSize.y >> gridSizeF >> layers;

			//this->gridSizeF = _gridSizeF;
			//this->gridSizeU = static_cast<uint32_t>(this->gridSizeF);

			this->Clear();

			this->map.resize(this->maxSize.x, std::vector<std::vector<Tile*>>());

			for (size_t x = 0; x < this->maxSize.x; x++)
			{
				for (size_t y = 0; y < this->maxSize.y; y++)
				{
					this->map[x].resize(this->maxSize.y, std::vector<Tile*>());

					for (size_t z = 0; z < this->layers; z++)
					{
						this->map[x][y].resize(this->layers, nullptr);
					}
				}
			}

			while (_loadFile >> maxSize.x >> maxSize.y >> layers >> _bColliding)
			{
				std::cout << maxSize.x << ", " << maxSize.y << ", " << layers << "\n";

				this->map[maxSize.x][maxSize.y][layers] = new Tile(maxSize.x, maxSize.y, gridSizeF, _bColliding);
			}
		}
		else
		{
			std::cout << "Error: TileMap could not load " << fileName << "\n";
		}

		_loadFile.close();
	}
};
ECS_DEFINE_TYPE(TileMap);