#include "Level.h"

#include <Otherwise/ErrorHandle.h>
#include <fstream>
#include <iostream>
#include <Otherwise/ResourceManager.h>



Level::Level() 
{
	//empty
}

Level::~Level()
{
}

void Level::loadLevel(const std::string & fileName)
{
	std::ifstream file;
	file.open(fileName);

	// Error checking
	if (file.fail())
	{
		Otherwise::fatalError("Failed to open " + fileName);
	}

	// Throw away the first string in tmp
	std::string tmp;

	file >> tmp >> _numHumans;

	std::getline(file, tmp); // Throw away the rest of the first line

							 // Read the level data
	while (std::getline(file, tmp))
	{
		_levelData.emplace_back(tmp);
	}

	prepareLevel();
	
}

void Level::prepareLevel()
{
_spriteBatch.init();
_spriteBatch.begin();

glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

Otherwise::ColorRGBA8 whiteColor;
whiteColor.r = 255;
whiteColor.g = 255;
whiteColor.b = 255;
whiteColor.a = 255;

// Render all the tiles
for (unsigned int y = 0; y < _levelData.size(); y++)
{
	for (unsigned int x = 0; x < _levelData[y].size(); x++)
	{
		// Grab the tile
		char tile = _levelData[y][x];

		// Get dest rect
		glm::vec4 destRect(x * TILE_WIDTH, y * TILE_WIDTH, TILE_WIDTH, TILE_WIDTH);

		glm::vec2 newSpawnPoint;

		// Process the tile
		switch (tile)
		{
		case 'S':
			newSpawnPoint.x = x * TILE_WIDTH;
			newSpawnPoint.y = y * TILE_WIDTH;
			mSpawnPoints.push_back(newSpawnPoint);
			_spriteBatch.draw(destRect,
				uvRect,
				Otherwise::ResourceManager::getTexture("Textures/tile_grass.png").id,
				0.0f,
				whiteColor);
			break;
		case 'G':
			_spriteBatch.draw(destRect,
				uvRect,
				Otherwise::ResourceManager::getTexture("Textures/tile_grass.png").id,
				0.0f,
				whiteColor);
			break;
		case 'R':
			_spriteBatch.draw(destRect,
				uvRect,
				Otherwise::ResourceManager::getTexture("Textures/bricks.png").id,
				0.0f,
				whiteColor);
			break;
			break;
		case '@':
			//_levelData[y][x] = '.'; /// So we dont collide with a @

			_spriteBatch.draw(destRect,
				uvRect,
				Otherwise::ResourceManager::getTexture("Textures/tile_grass.png").id,
				0.0f,
				whiteColor);

			_startPlayerPos.x = x * TILE_WIDTH;
			_startPlayerPos.y = y * TILE_WIDTH;
			break;
		case '.':
			_spriteBatch.draw(destRect,
				uvRect,
				Otherwise::ResourceManager::getTexture("Textures/tile_grass.png").id,
				0.0f,
				whiteColor);
			break;
		default:
			std::printf("Unexpected symbol %c at (%d,%d)", tile, x, y);
			_spriteBatch.draw(destRect,
				uvRect,
				Otherwise::ResourceManager::getTexture("Textures/tile_grass.png").id,
				0.0f,
				whiteColor);
			break;
		}
	}
}

_spriteBatch.end();
}

void Level::saveLevel(const std::string & fileName)
{
	std::ofstream file;
	file.open(fileName + ".txt");

	file << "Humans: " << _numHumans << std::endl;

	for (int y = 0; y < _levelData.size(); y++)
	{
		file << _levelData[y] << std::endl;
	}

	file.close();
}

void Level::draw() 
{
    _spriteBatch.renderBatch();
}

glm::vec2 Level::getRandSpawnPoint(std::mt19937 randomEngine)
{
	glm::vec2 spawnPoint;

	std::uniform_int_distribution<int> randSpawnNum(0, mSpawnPoints.size());

	spawnPoint = mSpawnPoints[randSpawnNum(randomEngine)];

	return spawnPoint;
}

void Level::replaceSingle(glm::vec2 position, std::string data)
{
	_levelData[position.y].replace(position.x, 1, data);
}

void Level::replaceRect(std::vector<std::string> data, glm::vec2 topLeft)
{
	for (unsigned int i = 0; i < data.size(); i++)
	{
		_levelData[topLeft.y + i].replace(topLeft.x, data[i].length(), data[i]);
	}
}
