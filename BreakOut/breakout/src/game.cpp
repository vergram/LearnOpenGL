#include "game.h"

#include "resource_manager.h"
#include "SpriteRenderer.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

namespace BreakOut {

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
		ResourceManager::LoadShader("breakout/res/shader/BasicSprite.shader", "sprite");
		glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(m_Width), static_cast<float>(m_Height), 0.0f, -1.0f, 1.0f);

		Shader& shader = ResourceManager::GetShader("sprite");
		shader.Bind();
		shader.SetUniform1i("image", 0);
		shader.SetUniformMatrix4fv("projection", projection);
		Renderer = new SpriteRenderer(shader);

		ResourceManager::LoadTexture("breakout/res/image/awesomeface.png", "smile");
	}

	void Game::ProcessInput(GLfloat deltaTime)
	{}

	void Game::OnUpdate(GLfloat deltaTime)
	{}

	void Game::Render()
	{
		Renderer->DrawSprite(ResourceManager::GetTexture("smile"), glm::vec2(200, 200), glm::vec2(300, 400), 45.0f, 
			glm::vec3(0.0f, 1.0f, 0.0f));
	}

}
