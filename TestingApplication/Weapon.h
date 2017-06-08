#pragma once

#include "Otherwise/spritebatch.h"
#include <Otherwise/AudioEngine.h>

#include "Ammo.h"
#include "Agent.h"

#include <vector>

class Weapon
{
public:
	Weapon();
	virtual ~Weapon();

	virtual void update() = 0;//forcing any weapon to update itself, so that it can check if it's been fired

	virtual void attack() = 0;//forcing any weapon to make an attack

	float getDamage() const { return mDamage; }//so that the agent we hit can take damage

protected:

	Otherwise::SoundEffect mSoundEffect;//the sound the weapon makes when it hits or fires

	std::string mName;//name of the weapon, in case we want to display it

	int mAttackRate; //Fire rate in terms of frames

	float mFrameCounter; //Counts frames so we know when to attack

	glm::vec2 mPosition;//where the weapon is, in case it isn't where the weilder is
	glm::vec2 mDirection;//the direction the weapon is pointing
	float mDamage;//how much damage the weapon does
};

