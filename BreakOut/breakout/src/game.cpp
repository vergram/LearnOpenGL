#include "game.h"

#include "resource_manager.h"
#include "SpriteRenderer.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <GLFW/glfw3.h>

namespace BreakOut {

	const glm::vec2 PLAYER_SIZE(100, 20);
	const float PLAYER_VELOCITY(500.0f);

	GameObject * Player;
	SpriteRenderer * Renderer;

	Game::Game(GLuint width, GLuint height)
		:m_State(GAME_ACTIVE),
		m_Keys(),
		m_Width(width),
		m_Height(height)
	{}

	Game::~Game()
	{}

	void Game::Init()
	{
		glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(m_Width), static_cast<float>(m_Height), 0.0f, -1.0f, 1.0f);

		// shader set up
		ResourceManager::LoadShader("breakout/res/shader/BasicSprite.shader", "sprite");
		Shader& shader = ResourceManager::GetShader("sprite");
		shader.Bind();
		shader.SetUniform1i("image", 0);
		shader.SetUniformMatrix4fv("projection", projection);
		Renderer = new SpriteRenderer(shader);

		// load Textures
		ResourceManager::LoadTexture("breakout/res/image/awesomeface.png", "smile");
		ResourceManager::LoadTexture("breakout/res/image/block.png"      , "block");
		ResourceManager::LoadTexture("breakout/res/image/block_solid.png", "block_solid");
		ResourceManager::LoadTexture("breakout/res/image/background.jpg" , "background");
		ResourceManager::LoadTexture("breakout/res/image/paddle.png"     , "player");

		// load levels
		GameLevel one;   one.Load("breakout/res/levels/1", m_Width, m_Height * 0.5f);
		GameLevel two;   two.Load("breakout/res/levels/2", m_Width, m_Height * 0.5f);
		GameLevel three; three.Load("breakout/res/levels/3", m_Width, m_Height * 0.5f);
		GameLevel four;  four.Load("breakout/res/levels/4", m_Width, m_Height * 0.5f);
		m_Levels.push_back(one);
		m_Levels.push_back(two);
		m_Levels.push_back(three);
		m_Levels.push_back(four);
		m_CurrentLevel = 1;

		glm::vec2 playerPos((m_Width - PLAYER_SIZE.x) / 2, m_Height - PLAYER_SIZE.y);
		Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("player"));

	}

	void Game::ProcessInput(GLfloat deltaTime)
	{
		if (m_State == GAME_ACTIVE)
		{
			float velocity = PLAYER_VELOCITY * deltaTime;
			if (m_Keys[GLFW_KEY_A])
			{
				if (Player->Position.x > 0)
				{
					Player->Position.x -= velocity;
				}
			}
			if (m_Keys[GLFW_KEY_D])
			{
				if (Player->Position.x < m_Width - PLAYER_SIZE.x)
				{
					Player->Position.x += velocity;
				}
			}
		}
	}

	void Game::OnUpdate(GLfloat deltaTime)
	{}

	void Game::Render()
	{
		if (m_State == GAME_ACTIVE)
		{
			Renderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0, 0), glm::vec2(m_Width, m_Height), 0.0f, 
				glm::vec3(1.0f));

			m_Levels[m_CurrentLevel - 1].Draw(*Renderer);

			Player->Draw(*Renderer);
		}
	}

}
