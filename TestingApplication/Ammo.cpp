#include "Ammo.h"


Ammo::Ammo()
{
}

Ammo::Ammo(glm::vec2 position, glm::vec2 direction, float damage, float speed)
{
}

Ammo::~Ammo()
{
}

bool Ammo::update(const std::vector<std::string>& levelData, float deltaTime)
{
	return false;
}

void Ammo::draw(Otherwise::SpriteBatch & spriteBatch)
{
}

bool Ammo::collideWithAgent(Agent * agent)
{
	return false;
}

bool Ammo::collideWithWorld(const std::vector<std::string>& levelData)
{
	return false;
}
