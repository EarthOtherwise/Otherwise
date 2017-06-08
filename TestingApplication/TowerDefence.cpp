#include "TowerDefence.h"

TowerDefence::TowerDefence()
{
}


TowerDefence::~TowerDefence()
{
}

void TowerDefence::init(Otherwise::GLSLProgram* textureProgram, Otherwise::InputManager* inputManager, 
	Otherwise::Camera2D* camera, Otherwise::Camera2D* hudCamera)
{
	m_textureProgram = textureProgram;
	m_inputManager = inputManager;
	m_camera = camera;
	m_hudCamera = hudCamera;

	// Initialize our spritebatch
	m_agentSpriteBatch.init();
	m_hudSpriteBatch.init();

	// Initialize sprite font
	m_spriteFont = new Otherwise::SpriteFont("Fonts/chintzy.ttf", 64);

	// Initialize sound, must happen after Otherwise::init
	m_audioEngine.init();

	//std::ifstream file;
	//file.open("Levels/levelDir.txt");

	// Error checking
	//if (file.fail()) {
		//Otherwise::fatalError("Failed to open Levels/levelDir.txt");
	//}

	std::string tmp;
	m_levels.push_back(new Level());
	m_levels.back()->loadLevel("Levels/towerDefenceMap.txt");

	m_currentLevel = 0;

	m_player = new Player();
	m_player->init(PLAYER_SPEED, m_levels[m_currentLevel]->getStartPlayerPos(), m_inputManager, m_camera, &m_bullets);

	m_humans.push_back(m_player);

	//std::mt19937 randomEngine;
	//randomEngine.seed(time(nullptr));

	//std::uniform_int_distribution<int> randX(2, m_levels[m_currentLevel]->getWidth() - 2);
	//std::uniform_int_distribution<int> randY(2, m_levels[m_currentLevel]->getHeight() - 2);

	// Add all the random humans
	//for (int i = 0; i < m_levels[m_currentLevel]->getNumHumans(); i++)
	//{
		//m_humans.push_back(new Human);
		//glm::vec2 pos(randX(randomEngine) * TILE_WIDTH, randY(randomEngine) * TILE_WIDTH);
		//m_humans.back()->init(HUMAN_SPEED, pos);
	//}

	// Set up the players guns
	const float BULLET_SPEED = 20.0f;
	m_player->addGun(new Gun("Magnum", 10, 1, 5.0f / 57.3f, 30, BULLET_SPEED, m_audioEngine.loadSoundEffect("Sound/shots/pistol.wav")));
	m_player->addGun(new Gun("Shotgun", 30, 12, 20.0f / 57.3f, 4, BULLET_SPEED, m_audioEngine.loadSoundEffect("Sound/shots/shotgun.wav")));
	m_player->addGun(new Gun("MP5", 2, 1, 10.0f / 57.3f, 20, BULLET_SPEED, m_audioEngine.loadSoundEffect("Sound/shots/cg1.wav")));

	Otherwise::Music music = m_audioEngine.loadMusic("Sound/XYZ.ogg");
	music.play(-1);

	// Used to cap the FPS
	fpsLimiter.setMaxFPS(60.0f);

	// Zoom out the camera by 3x
	float cameraScale = 1.0f / 3.0f;

	// Start our previousTicks variable
	previousTicks = SDL_GetTicks();
}

GameState TowerDefence::run()
{
	mRandomEngine.seed(time(nullptr));

	fpsLimiter.begin();

	// Calculate the frameTime in milliseconds
	float newTicks = SDL_GetTicks();
	float frameTime = newTicks - previousTicks;
	previousTicks = newTicks; // Store newTicks in previousTicks so we can use it next frame
								// Get the total delta time
	float totalDeltaTime = frameTime / DESIRED_FRAMETIME;

	// TODO: Support for multiple levels!
	// _currentLevel++; initLevel(...);

	// If all zombies are dead we win!
	//if (m_zombies.empty())
	//{
		// Print victory message
	//	std::printf("*** You win! ***\n You killed %d humans and %d zombies. There are %d/%d civilians remaining",
		//	m_numHumansKilled, m_numZombiesKilled, m_humans.size() - 1, m_levels[m_currentLevel]->getNumHumans());

		// Easy way to end the game :P
	//	Otherwise::fatalError("");
//	}

	//if there are fewer than 100 zombies, spawn a new one.
// Add the zombies
//const std::vector<glm::vec2>& zombiePositions = m_levels[m_currentLevel]->getZombieStartPositions();
	tempTime += 0.01f;
	if (m_zombies.size() < 100 && tempTime > 1.0f)
	{
		tempTime = 0.0f;
		m_zombies.push_back(new Zombie);
		m_zombies.back()->init(ZOMBIE_SPEED, m_levels[m_currentLevel]->getRandSpawnPoint(mRandomEngine));
	}

	m_inputManager->update();

	SDL_Event evnt;
	//Will keep looping until there are no more events to process
	while (SDL_PollEvent(&evnt))
	{
		switch (evnt.type)
		{
		case SDL_QUIT:
			return GameState::EXIT;
			break;
		case SDL_MOUSEMOTION:
			m_inputManager->setMouseCoords(evnt.motion.x, evnt.motion.y);
			break;
		case SDL_KEYDOWN:
			m_inputManager->pressKey(evnt.key.keysym.sym);
			break;
		case SDL_KEYUP:
			m_inputManager->releaseKey(evnt.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			m_inputManager->pressKey(evnt.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			m_inputManager->releaseKey(evnt.button.button);
			break;
		}
	}

	int i = 0; // This counter makes sure we don't spiral to death!
				// Loop while we still have steps to process.
	while (totalDeltaTime > 0.0f && i < MAX_PHYSICS_STEPS)
	{
		// The deltaTime should be the the smaller of the totalDeltaTime and MAX_DELTA_TIME
		float deltaTime = std::min(totalDeltaTime, MAX_DELTA_TIME);

		// Update all physics here and pass in deltaTime

		// Update all humans
		for (unsigned int i = 0; i < m_humans.size(); i++)
		{
			m_humans[i]->update(m_levels[m_currentLevel]->getLevelData(),
				m_humans,
				m_zombies,
				deltaTime);
		}

		// Update all zombies
		for (unsigned int i = 0; i < m_zombies.size(); i++)
		{
			m_zombies[i]->update(m_levels[m_currentLevel]->getLevelData(),
				m_humans,
				m_zombies,
				deltaTime);
		}

		// Update Zombie collisions
		for (unsigned int i = 0; i < m_zombies.size(); i++)
		{
			// Collide with other zombies
			for (unsigned int j = i + 1; j < m_zombies.size(); j++)
			{
				m_zombies[i]->collideWithAgent(m_zombies[j]);
			}
			// Collide with humans
			for (unsigned int j = 1; j < m_humans.size(); j++)
			{
				if (m_zombies[i]->collideWithAgent(m_humans[j]))
				{
					// Add the new zombie
					m_zombies.push_back(new Zombie);
					m_zombies.back()->init(ZOMBIE_SPEED, m_humans[j]->getPosition());
					// Delete the human
					delete m_humans[j];
					m_humans[j] = m_humans.back();
					m_humans.pop_back();
				}
			}

			// Collide with player
			if (m_zombies[i]->collideWithAgent(m_player))
			{
				Otherwise::fatalError("YOU LOSE");
			}
		}

		// Update Human collisions
		for (unsigned int i = 0; i < m_humans.size(); i++)
		{
			// Collide with other humans
			for (unsigned int j = i + 1; j < m_humans.size(); j++)
			{
				m_humans[i]->collideWithAgent(m_humans[j]);
			}
		}

		// Dont forget to update zombies





		// Update and collide with world
		for (unsigned int i = 0; i < m_bullets.size(); )
		{
			// If update returns true, the bullet collided with a wall
			if (m_bullets[i].update(m_levels[m_currentLevel]->getLevelData(), deltaTime))
			{
				m_bullets[i] = m_bullets.back();
				m_bullets.pop_back();
			}
			else
			{
				i++;
			}
		}

		bool wasBulletRemoved;

		// Collide with humans and zombies
		for (unsigned int i = 0; i < m_bullets.size(); i++)
		{
			wasBulletRemoved = false;
			// Loop through zombies
			for (unsigned int j = 0; j < m_zombies.size(); )
			{
				// Check collision
				if (m_bullets[i].collideWithAgent(m_zombies[j]))
				{
					// Damage zombie, and kill it if its out of health
					if (m_zombies[j]->applyDamage(m_bullets[i].getDamage()))
					{
						// If the zombie died, remove him
						delete m_zombies[j];
						m_zombies[j] = m_zombies.back();
						m_zombies.pop_back();
						m_numZombiesKilled++;
					}
					else
					{
						j++;
					}

					// Remove the bullet
					m_bullets[i] = m_bullets.back();
					m_bullets.pop_back();
					wasBulletRemoved = true;
					i--; // Make sure we don't skip a bullet
							// Since the bullet died, no need to loop through any more zombies
					break;
				}
				else
				{
					j++;
				}
			}
			// Loop through humans
			if (wasBulletRemoved == false)
			{
				for (unsigned int j = 1; j < m_humans.size(); )
				{
					// Check collision
					if (m_bullets[i].collideWithAgent(m_humans[j]))
					{
						// Damage human, and kill it if its out of health
						if (m_humans[j]->applyDamage(m_bullets[i].getDamage()))
						{
							// If the human died, remove him
							delete m_humans[j];
							m_humans[j] = m_humans.back();
							m_humans.pop_back();
						}
						else
						{
							j++;
						}

						// Remove the bullet
						m_bullets[i] = m_bullets.back();
						m_bullets.pop_back();
						m_numHumansKilled++;
						i--; // Make sure we don't skip a bullet
								// Since the bullet died, no need to loop through any more zombies
						break;
					}
					else
					{
						j++;
					}
				}
			}
		}

		// Since we just took a step that is length deltaTime, subtract from totalDeltaTime
		totalDeltaTime -= deltaTime;
		// Increment our frame counter so we can limit steps to MAX_PHYSICS_STEPS
		i++;
	}

	if (m_inputManager->isKeyPressed(SDLK_z))
	{
		cameraScale += cameraScale / 3.0f;
		m_camera->setScale(cameraScale);
	}
	else if (m_inputManager->isKeyPressed(SDLK_x))
	{
		cameraScale -= cameraScale / 3.0f;
		m_camera->setScale(cameraScale);
	}
	else if (m_inputManager->isKeyPressed(SDLK_l))
	{
		return GameState::TOWERBUILDER;
	}

	// Make sure the camera is bound to the player position
	m_camera->setPosition(m_player->getPosition());

	m_camera->update();
	m_hudCamera->update();

	// Set the base depth to 1.0
	glClearDepth(1.0);
	// Clear the color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_textureProgram->use();

	// Draw code goes here
	glActiveTexture(GL_TEXTURE0);

	// Make sure the shader uses texture 0
	GLint textureUniform = m_textureProgram->getUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);

	// Grab the camera matrix
	glm::mat4 projectionMatrix = m_camera->getCameraMatrix();
	GLint pUniform = m_textureProgram->getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	// Draw the level
	m_levels[m_currentLevel]->draw();

	// Begin drawing agents
	m_agentSpriteBatch.begin();

	const glm::vec2 agentDims(AGENT_RADIUS * 2.0f);

	// Draw the humans
	for (unsigned int i = 0; i < m_humans.size(); i++)
	{
		if (m_camera->isBoxInView(m_humans[i]->getPosition(), agentDims))
		{
			m_humans[i]->draw(m_agentSpriteBatch);
		}
	}

	// Draw the zombies
	for (unsigned int i = 0; i < m_zombies.size(); i++)
	{
		if (m_camera->isBoxInView(m_zombies[i]->getPosition(), agentDims))
		{
			m_zombies[i]->draw(m_agentSpriteBatch);
		}
	}

	// Draw the bullets
	for (unsigned int i = 0; i < m_bullets.size(); i++)
	{
		m_bullets[i].draw(m_agentSpriteBatch);
	}

	// End spritebatch creation
	m_agentSpriteBatch.end();

	// Render to the screen
	m_agentSpriteBatch.renderBatch();

	// Render the heads up display
	char buffer[256];

	glm::mat4 hudProjectionMatrix = m_hudCamera->getCameraMatrix();
	GLint hudPUniform = m_textureProgram->getUniformLocation("P");
	glUniformMatrix4fv(hudPUniform, 1, GL_FALSE, &hudProjectionMatrix[0][0]);

	m_hudSpriteBatch.begin();

	sprintf_s(buffer, "Num Humans %d", m_humans.size());
	m_spriteFont->draw(m_hudSpriteBatch, buffer, glm::vec2(0, 0),
		glm::vec2(0.5), 0.0f, Otherwise::ColorRGBA8(255, 255, 255, 255));

	sprintf_s(buffer, "Num Zombies %d", m_zombies.size());
	m_spriteFont->draw(m_hudSpriteBatch, buffer, glm::vec2(0, 36),
		glm::vec2(0.5), 0.0f, Otherwise::ColorRGBA8(255, 255, 255, 255));

	m_hudSpriteBatch.end();
	m_hudSpriteBatch.renderBatch();

	// Unbind the program
	m_textureProgram->unuse();

	// End the frame, limit the FPS, and get the current FPS.
	m_fps = fpsLimiter.end();

	return GameState::TOWERDEFENCE;
}

void TowerDefence::exit()
{
	// Don't forget to delete the levels!
	for (unsigned int i = 0; i < m_levels.size(); i++) {
		delete m_levels[i];
	}
	// Don't forget to delete the humans and zombies!
	for (unsigned int i = 0; i < m_humans.size(); i++) {
		delete m_humans[i];
	}
	for (unsigned int i = 0; i < m_zombies.size(); i++) {
		delete m_zombies[i];
	}
	m_audioEngine.destroy();
}
