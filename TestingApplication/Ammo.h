#pragma once

#include <vector>
#include <glm\glm.hpp>

#include "Otherwise\SpriteBatch.h"

#include "Agent.h"

class Ammo
{
public:
	Ammo();
	Ammo(glm::vec2 position, glm::vec2 direction, float damage, float speed);
	~Ammo();

	// When update returns true, delete bullet
	bool update(const std::vector<std::string>& levelData, float deltaTime);

	void draw(Otherwise::SpriteBatch& spriteBatch);

	bool collideWithAgent(Agent* agent);

	float getDamage() const { return mDamage; }

	glm::vec2 getPosition() const { return mPosition; }

protected:

	bool collideWithWorld(const std::vector<std::string>& levelData);

	bool checkDistance();

	glm::vec2 mPosition;
	glm::vec2 mOriginalPosition;
	glm::vec2 mDirection;
	float mDamage;
	float mSpeed;
	float mDistance;
};

