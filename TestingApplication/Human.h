#pragma once

#include "Agent.h"
#include "Gun.h"

class Human : public Agent
{
public:
    Human();
    virtual ~Human();

    void init(float speed, glm::vec2 pos);

	void addGun(Gun* gun);

    virtual void update(const std::vector<std::string>& levelData,
                        std::vector<Human*>& humans,
                        std::vector<Zombie*>& zombies,
                        float deltaTime) override;

private:
    int _frames;
	bool mRetreat = false;
	bool mRetreatPath = false;
	unsigned int mArrows = 0;
	std::vector<glm::vec2> mPath;
	unsigned int mCurrentPoint = 0;
	glm::vec2 mSafePosition;
	float mHealth;
	float mTotalHealth;
	std::vector<Gun*> _guns;
};

