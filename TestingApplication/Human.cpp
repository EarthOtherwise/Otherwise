#include "Human.h"
#include <ctime>
#include <random>
#include <glm/gtx/rotate_vector.hpp>
#include <Otherwise/ResourceManager.h>
#include <Otherwise/PathFinding.h>

Human::Human() :
    _frames(0)
{
}


Human::~Human()
{
}

void Human::init(float speed, glm::vec2 pos) 
{
    static std::mt19937 randomEngine(time(nullptr));
    static std::uniform_real_distribution<float> randDir(-1.0f, 1.0f);

    _health = 20;

    _color.r = 255;
    _color.g = 255;
    _color.b = 255;
    _color.a = 255;

    _speed = speed;
    _position = pos;
    // Get random direction
    m_direction = glm::vec2(randDir(randomEngine), randDir(randomEngine));
    // Make sure direction isn't zero
    if (m_direction.length() == 0) m_direction = glm::vec2(1.0f, 0.0f);

    m_direction = glm::normalize(m_direction);

    m_textureID = Otherwise::ResourceManager::getTexture("Textures/human.png").id;
}

void Human::update(const std::vector<std::string>& levelData,
                   std::vector<Human*>& humans,
                   std::vector<Zombie*>& zombies,
                   float deltaTime)
{
	/*if (!mRetreat && _position != mSafePosition)
	{
		if (non human is inside firing arc && mArrows > 0)
		{
			m_direction = the nearest non humans _position
			fire bow
		}
		else if (mHealth < mTotalHealth)
		{
			mRetreat = true;
		}
	}
	else if (!mRetreatPath)
	{
		path to the safe zone
	}
	else
	{
		if ((mPath[mCurrentPoint].x - _position.x) < _speed && (mPath[mCurrentPoint].y - _position.y) < _speed && mCurrentPoint < mPath.size())
		{
			mCurrentPoint++;
		}
		_position += mPath[mCurrentPoint] * _speed * deltaTime;
	}*/
}

void Human::addGun(Gun* gun)
{
	// Add the gun to his inventory
	_guns.push_back(gun);

	// If no gun equipped, equip gun.
	//if (_currentGunIndex == -1)
	//{
		//_currentGunIndex = 0;
	//}
}