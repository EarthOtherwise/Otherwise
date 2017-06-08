#include <Otherwise/Otherwise.h>
#include <Otherwise/Window.h>
#include <Otherwise/GLSLProgram.h>
#include <Otherwise/Camera2D.h>
#include <Otherwise/InputManager.h>

#include <SDL/SDL.h>
#include <iostream>
#include <glm/gtx/rotate_vector.hpp>

#include "MainMenu.h"
#include "TowerDefence.h"
#include "MapEdit.h"

#include "global.h"

int main(int argc, char** argv)
{
	/// Member Variables
	Otherwise::Window m_window; ///< The game window

	Otherwise::GLSLProgram m_textureProgram; ///< The shader program

	Otherwise::InputManager m_inputManager; ///< Handles input

	Otherwise::Camera2D m_camera; ///< Main Camera
	Otherwise::Camera2D m_hudCamera; ///< Hud Camera

	MainMenu mainMenu;
	TowerDefence towerDefence;
	MapEdit towerBuilder;

	int m_screenWidth = 1024;
	int m_screenHeight = 700;

	GameState m_gameState = GameState::TOWERBUILDER;

	// Initialize the game engine
	Otherwise::init();

	// Create our window
	m_window.create("Kuiper", m_screenWidth, m_screenHeight, 0);

	// Grey background color
	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);

	// Set up the shaders
	// Compile our color shader
	m_textureProgram.compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
	m_textureProgram.addAttribute("vertexPosition");
	m_textureProgram.addAttribute("vertexColor");
	m_textureProgram.addAttribute("vertexUV");
	m_textureProgram.linkShaders();

	// Set up the camera
	m_camera.init(m_screenWidth, m_screenHeight);
	m_hudCamera.init(m_screenWidth, m_screenHeight);
	m_hudCamera.setPosition(glm::vec2(m_screenWidth / 2, m_screenHeight / 2));

	towerBuilder.init(&m_textureProgram, &m_inputManager, &m_camera, &m_hudCamera);

	//run game
	while (m_gameState != GameState::EXIT)
	{
		switch (m_gameState)
		{
		case GameState::MAINMENU:
			m_gameState = mainMenu.run();
			if (m_gameState != GameState::MAINMENU)
			{
				mainMenu.exit();
				if (m_gameState == GameState::TOWERBUILDER)
				{
					towerBuilder.init(&m_textureProgram, &m_inputManager, &m_camera, &m_hudCamera);
				}
				else if (m_gameState == GameState::TOWERDEFENCE)
				{
					towerDefence.init(&m_textureProgram, &m_inputManager, &m_camera, &m_hudCamera);
				}
			}
			break;
		case GameState::TOWERBUILDER:
			m_gameState = towerBuilder.run();
			if (m_gameState != GameState::TOWERBUILDER)
			{
				towerBuilder.exit();
				if (m_gameState == GameState::MAINMENU)
				{
					mainMenu.init(&m_textureProgram, &m_inputManager, &m_camera, &m_hudCamera);
				}
				else if (m_gameState == GameState::TOWERDEFENCE)
				{
					towerDefence.init(&m_textureProgram, &m_inputManager, &m_camera, &m_hudCamera);
				}
			}
			break;
		case GameState::TOWERDEFENCE:
			m_gameState = towerDefence.run();
			if (m_gameState != GameState::TOWERDEFENCE)
			{
				towerDefence.exit();
				if (m_gameState == GameState::TOWERBUILDER)
				{
					towerBuilder.init(&m_textureProgram, &m_inputManager, &m_camera, &m_hudCamera);
				}
				else if (m_gameState == GameState::MAINMENU)
				{
					mainMenu.init(&m_textureProgram, &m_inputManager, &m_camera, &m_hudCamera);
				}
			}
			break;
		}

		// Swap our buffer and draw everything to the screen!
		m_window.swapBuffer();
		std::cout << "Frame" << std::endl;
	}
	




    return 0;
}