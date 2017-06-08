#include "Bow.h"

Bow::Bow(std::string name, int fireRate, float spread, float damage, float speed, Otherwise::SoundEffect fireEffect)
{
	mName = name;
	mAttackRate = fireRate;
	mSpread = spread;
	mBowDamage = damage;
	mBowSpeed = speed;
	mFrameCounter = 0;
	mSoundEffect = fireEffect;
}

Bow::~Bow()
{
}

void Bow::update(bool isMouseDown, const glm::vec2& position, const glm::vec2& direction, std::vector<Arrow>& arrows, float deltaTime)
{
	mFrameCounter += 1.0f * deltaTime;
	// After a certain number of frames has passed we fire our gun
	if (mFrameCounter >= mAttackRate && isMouseDown)
	{
		//Attack(direction, position, arrows);
		mFrameCounter = 0;
	}
}

void Bow::attack(const glm::vec2& direction, const glm::vec2& position, std::vector<Arrow>& bullets)
{
}
