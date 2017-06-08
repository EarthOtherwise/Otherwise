#include "MapEdit.h"

#include <SDL/SDL.h>
#include <iostream>
#include <fstream>

MapEdit::MapEdit()
{
}

MapEdit::~MapEdit()
{
}

void MapEdit::init(Otherwise::GLSLProgram * textureProgram, Otherwise::InputManager * inputManager, Otherwise::Camera2D * camera, Otherwise::Camera2D * hudCamera)
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

	// Throw away the first string in tmp
	std::string tmp;

	m_levels.push_back(new Level());
	m_levels.back()->loadLevel("Levels/towerDefenceMap.txt");

	std::ifstream file;

	file.open("MapModules/moduleDir.txt");

	// Error checking
	if (file.fail()) {
		Otherwise::fatalError("MapModules/moduleDir.txt");
	}

	while (std::getline(file, tmp))
	{
		mTowers.push_back(new Level());
		mTowers.back()->loadLevel(tmp);
	}

	file.close();

	file.open("MapModules/tileDir.txt");

	// Error checking
	if (file.fail()) {
		Otherwise::fatalError("MapModules/tileDir.txt");
	}

	while (std::getline(file, tmp))
	{
		mTileStrings.push_back(tmp);
	}

	file.close();

	Otherwise::Music music = m_audioEngine.loadMusic("Sound/XYZ.ogg");
	music.play(-1);

	// Used to cap the FPS
	fpsLimiter.setMaxFPS(60.0f);

	// Zoom out the camera by 3x
	float cameraScale = 0.3f;

	// Start our previousTicks variable
	previousTicks = SDL_GetTicks();

	mCameraPosition = m_levels[currentLevel]->getStartPlayerPos();
}

GameState MapEdit::run()
{
	fpsLimiter.begin();

	// Calculate the frameTime in milliseconds
	float newTicks = SDL_GetTicks();
	float frameTime = newTicks - previousTicks;
	previousTicks = newTicks; // Store newTicks in previousTicks so we can use it next frame
							  // Get the total delta time
	float totalDeltaTime = frameTime / DESIRED_FRAMETIME;

	// TODO: Support for multiple levels!
	// _currentLevel++; initLevel(...);

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

	if (m_inputManager->isKeyPressed(SDLK_z) && cameraScale < 10.0f)
	{
		cameraScale += cameraScale/3.0f;
		m_camera->setScale(cameraScale);
	}
	if (m_inputManager->isKeyPressed(SDLK_x) && cameraScale > 0.1f)
	{
		cameraScale -= cameraScale / 3.0f;
		m_camera->setScale(cameraScale);
	}
	if (m_inputManager->isKeyDown(SDLK_w))
	{
		mCameraPosition.y += 10.0f / cameraScale;
	}
	if (m_inputManager->isKeyDown(SDLK_a))
	{
		mCameraPosition.x -= 10.0f / cameraScale;
	}
	if (m_inputManager->isKeyDown(SDLK_s))
	{
		mCameraPosition.y -= 10.0f / cameraScale;
	}
	if (m_inputManager->isKeyDown(SDLK_d))
	{
		mCameraPosition.x += 10.0f / cameraScale;
	}
	if (m_inputManager->isKeyDown(SDLK_l))
	{
		return GameState::TOWERDEFENCE;
	}
	if (m_inputManager->isKeyPressed(SDLK_k))
	{
		if (mainMap)
		{
			mainMap = false;
		}
		else
		{
			mainMap = true;
		}
	}


	if (m_inputManager->isKeyDown(SDLK_1))
	{
		mReplacementTile = 1;
	}
	if (m_inputManager->isKeyDown(SDLK_2))
	{
		mReplacementTile = 2;
	}
	if (m_inputManager->isKeyDown(SDLK_3))
	{
		mReplacementTile = 3;
	}
	if (m_inputManager->isKeyDown(SDLK_4))
	{
		mReplacementTile = 4;
	}
	if (m_inputManager->isKeyDown(SDLK_5))
	{
		mReplacementTile = 5;
	}
	if (m_inputManager->isKeyDown(SDLK_6))
	{
		mReplacementTile = 6;
	}
	if (m_inputManager->isKeyDown(SDLK_7))
	{
		mReplacementTile = 7;
	}
	if (m_inputManager->isKeyDown(SDLK_8))
	{
		mReplacementTile = 8;
	}
	if (m_inputManager->isKeyDown(SDLK_9))
	{
		mReplacementTile = 9;
	}
	if (m_inputManager->isKeyDown(SDLK_0))
	{
		mReplacementTile = 0;
	}



	if (m_inputManager->isKeyPressed(SDL_BUTTON_LEFT))
	{
		
		glm::vec2 mouseCoords = m_inputManager->getMouseCoords();//get the mouse coordinates
		
		mouseCoords = m_camera->convertScreenToWorld(mouseCoords);//change the screen coordinates to coordinates in the game world
		mouseCoords.x /= TILE_WIDTH;
		mouseCoords.y /= TILE_WIDTH;

		if (!mainMap && mouseCoords.x < mTowers[currentTower]->getWidth() && mouseCoords.y < mTowers[currentTower]->getHeight())
		{
			mTowers[currentTower]->replaceSingle(mouseCoords, mTileStrings[mReplacementTile]);
		}
		else if (mouseCoords.x < m_levels[currentLevel]->getWidth() && mouseCoords.y < m_levels[currentLevel]->getHeight())
		{
			std::cout << "Left click" << std::endl;
			m_levels[currentLevel]->replaceRect(mTowers[currentTower]->getLevelData(), mouseCoords);
			//m_levels[currentLevel]->saveLevel("Levels/newLevel.txt");
			//m_levels.pop_back();
			//m_levels.push_back(new Level());
			//m_levels.back()->loadLevel("Levels/newLevel.txt");
			m_levels[currentLevel]->prepareLevel();
		}
	}

	

	// Make sure the camera is bound to the player position
	m_camera->setPosition(mCameraPosition);

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
	if (mainMap)
	{
		m_levels[m_currentLevel]->draw();
	}
	else
	{
		mTowers[currentTower]->draw();
	}

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

	m_hudSpriteBatch.end();
	m_hudSpriteBatch.renderBatch();

	// Unbind the program
	m_textureProgram->unuse();

	// End the frame, limit the FPS, and get the current FPS.
	m_fps = fpsLimiter.end();

	return GameState::TOWERBUILDER;
}

void MapEdit::exit()
{
	m_levels[currentLevel]->saveLevel("Levels/towerDefenceMap");
	// Don't forget to delete the levels!
	for (unsigned int i = 0; i < m_levels.size(); i++) {
		delete m_levels[i];
	}
	// Don't forget to delete the humans!
	for (unsigned int i = 0; i < m_humans.size(); i++) {
		delete m_humans[i];
	}
	m_audioEngine.destroy();
}