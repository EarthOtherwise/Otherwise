#pragma once

#include "Weapon.h"
#include "Arrow.h"

class Bow : public Weapon
{
public:
	Bow(std::string name, int fireRate, float spread, float damage, float arrowSpeed, Otherwise::SoundEffect fireEffect);
	~Bow();

	void update(bool isMouseDown, const glm::vec2& position, const glm::vec2& direction, std::vector<Arrow>& arrows, float deltaTime);

	void attack(const glm::vec2& direction, const glm::vec2& position, std::vector<Arrow>& bullets);

private:
	float mSpread;
	float mBowDamage;
	float mBowSpeed;
};

