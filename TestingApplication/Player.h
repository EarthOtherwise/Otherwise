#pragma once

#include "Human.h"
#include <Otherwise/InputManager.h>
#include <Otherwise/Camera2D.h>
#include "Bullet.h"

class Gun;

class Player : public Human
{
public:
    Player();
    ~Player();

    void init(float speed, glm::vec2 pos, Otherwise::InputManager* inputManager, Otherwise::Camera2D* camera, std::vector<Bullet>* bullets);

    void addGun(Gun* gun);

    void update(const std::vector<std::string>& levelData,
                std::vector<Human*>& humans,
                std::vector<Zombie*>& zombies,
                float deltaTime) override;
private:
    Otherwise::InputManager* _inputManager;

    std::vector<Gun*> _guns;
    int _currentGunIndex;

    Otherwise::Camera2D* _camera;
    std::vector<Bullet>* _bullets;

};

