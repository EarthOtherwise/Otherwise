#pragma once

#include <Otherwise/SpriteBatch.h>
#include <Otherwise/SpriteFont.h>
#include <Otherwise/ErrorHandle.h>
#include <Otherwise/AudioEngine.h>
#include <Otherwise/Timing.h>
#include <Otherwise/ResourceManager.h>
#include <Otherwise/GLSLProgram.h>

#include <fstream>
#include <random>
#include <ctime>
#include <algorithm>

#include "global.h"
#include "Player.h"
#include "Level.h"
#include "Bullet.h"
#include "Gun.h"
#include "Zombie.h"

class MainMenu
{
public:
	MainMenu();
	~MainMenu();

	void init(Otherwise::GLSLProgram* textureProgram, Otherwise::InputManager* inputManager,
		Otherwise::Camera2D* camera, Otherwise::Camera2D* hudCamera);

	GameState run();

	void exit();

private:
	// Some helpful constants.
	const float DESIRED_FPS = 60.0f; // FPS the game is designed to run at
	const int MAX_PHYSICS_STEPS = 6; // Max number of physics steps per frame
	const float MS_PER_SECOND = 1000; // Number of milliseconds in a second
	const float DESIRED_FRAMETIME = MS_PER_SECOND / DESIRED_FPS; // The desired frame time per frame
	const float MAX_DELTA_TIME = 1.0f; // Maximum size of deltaTime

	const float HUMAN_SPEED = 1.0f;
	const float ZOMBIE_SPEED = 1.3f;
	const float PLAYER_SPEED = 5.0f;

	int m_currentLevel;

	Player* m_player = nullptr;
	std::vector<Human*> m_humans; ///< Vector of all humans
	std::vector<Zombie*> m_zombies; ///< Vector of all zombies
	std::vector<Bullet> m_bullets;

	int m_numHumansKilled = 0; ///< Humans killed by player
	int m_numZombiesKilled = 0; ///< Zombies killed by player

	Otherwise::SpriteBatch m_agentSpriteBatch; ///< Draws all agents
	Otherwise::SpriteBatch m_hudSpriteBatch;

	std::vector<Level*> m_levels; ///< vector of all levels

	Otherwise::SpriteFont* m_spriteFont;

	Otherwise::AudioEngine m_audioEngine;

	Otherwise::FpsLimiter fpsLimiter;

	float previousTicks;

	float cameraScale = 3.0f;
	glm::vec2 mCameraPosition;

	float m_fps = 0;

	Otherwise::GLSLProgram* m_textureProgram;
	Otherwise::InputManager* m_inputManager;
	Otherwise::Camera2D* m_camera;
	Otherwise::Camera2D* m_hudCamera;
};

