#include "MainMenu.h"

MainMenu::MainMenu()
{
}


MainMenu::~MainMenu()
{
}

void MainMenu::init(Otherwise::GLSLProgram * textureProgram, Otherwise::InputManager * inputManager, Otherwise::Camera2D * camera, Otherwise::Camera2D * hudCamera)
{
	m_textureProgram = textureProgram;
	m_inputManager = inputManager;
	m_camera = camera;
	m_hudCamera = hudCamera;

	m_hudSpriteBatch.init();

	// Initialize sprite font
	m_spriteFont = new Otherwise::SpriteFont("Fonts/chintzy.ttf", 64);

	// Initialize sound, must happen after Otherwise::init
	m_audioEngine.init();

	Otherwise::Music music = m_audioEngine.loadMusic("Sound/XYZ.ogg");
	music.play(-1);

	// Used to cap the FPS
	fpsLimiter.setMaxFPS(60.0f);

	// Zoom out the camera by 3x
	float cameraScale = 1.0f / 3.0f;

	// Start our previousTicks variable
	previousTicks = SDL_GetTicks();
}

GameState MainMenu::run()
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

	//what the buttons do

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
	m_levels[m_currentLevel]->draw();

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

	return GameState::MAINMENU;
}

void MainMenu::exit()
{
}
