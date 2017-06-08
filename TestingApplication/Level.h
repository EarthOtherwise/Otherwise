#pragma once

#include <string>
#include <vector>
#include <random>
#include <ctime>

#include <Otherwise/SpriteBatch.h>

const int TILE_WIDTH = 64;

class Level
{
public:
    // Load the level
    Level();
    ~Level();

	void loadLevel(const std::string& fileName);
	void saveLevel(const std::string& fileName);

    void draw();

	void prepareLevel();

    // Getters
    int getWidth() const { return _levelData[0].size(); }
    int getHeight() const { return _levelData.size(); }
    int getNumHumans() const { return _numHumans; }
    const std::vector<std::string>& getLevelData() const { return _levelData; }
    glm::vec2 getStartPlayerPos() const { return _startPlayerPos; }
    const std::vector<glm::vec2>& getZombieStartPositions() const { return _zombieStartPositions; }

	glm::vec2 getRandSpawnPoint(std::mt19937 randomEngine);

	void replaceSingle(glm::vec2 position, std::string data);
	void replaceRect(std::vector<std::string> data, glm::vec2 topLeft);

private:
	std::vector<glm::vec2> mSpawnPoints;
    std::vector<std::string> _levelData;
    int _numHumans;
    Otherwise::SpriteBatch _spriteBatch;

    glm::vec2 _startPlayerPos;
    std::vector<glm::vec2> _zombieStartPositions;
};

