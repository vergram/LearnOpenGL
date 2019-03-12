#include "game.h"

#include "resource_manager.h"
#include "SpriteRenderer.h"
#include "ball_object_collisions.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <GLFW/glfw3.h>

#include <algorithm>

namespace BreakOut {

	const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
	const float BALL_RADIUS = 12.5f;
	BallObject *Ball;

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
		ResourceManager::LoadTexture("breakout/res/image/awesomeface.png", "ball");
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
		m_CurrentLevel = 0;

		// player initialize
		glm::vec2 playerPos((m_Width - PLAYER_SIZE.x) / 2.0f, m_Height - PLAYER_SIZE.y);
		Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("player"));

		// ball initialize
		glm::vec2 ballPos(m_Width / 2.0f - BALL_RADIUS, m_Height - PLAYER_SIZE.y - BALL_RADIUS - BALL_RADIUS);
		Ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY, ResourceManager::GetTexture("ball"));
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
					if (Ball->m_Stuck)
					{
						Ball->Position.x -= velocity;
					}
				}
			}
			if (m_Keys[GLFW_KEY_D])
			{
				if (Player->Position.x < m_Width - PLAYER_SIZE.x)
				{
					Player->Position.x += velocity;
					if (Ball->m_Stuck)
					{
						Ball->Position.x += velocity;
					}
				}
			}
			if (m_Keys[GLFW_KEY_SPACE])
			{
				Ball->m_Stuck = false;
			}
		}
	}

	void Game::OnUpdate(GLfloat deltaTime)
	{
		Ball->Move(deltaTime, m_Width);
		DoCollisions();

		if (Ball->Position.y >= m_Height)
		{
			ResetLevel();
			ResetPlayer();
		}
	}

	void Game::Render()
	{
		if (m_State == GAME_ACTIVE)
		{
			Renderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0, 0), glm::vec2(m_Width, m_Height), 0.0f, 
				glm::vec3(1.0f));

			m_Levels[m_CurrentLevel].Draw(*Renderer);

			Player->Draw(*Renderer);

			Ball->Draw(*Renderer);
		}
	}


	Direction VectorDirection(glm::vec2 circleToClosest);
	bool CheckCollision(GameObject& a, GameObject& b);
	Collision CheckCollision(BallObject& ball, GameObject& b);

	void Game::DoCollisions()
	{
		for (GameObject& box : m_Levels[m_CurrentLevel].Bricks)
		{
			if (!box.Destroyed)
			{
				Collision collision = CheckCollision(*Ball, box);
				if (std::get<0>(collision))                      // is collision?
				{
					if (!box.IsSolid)
						box.Destroyed = true;
					Direction dir = std::get<1>(collision);
					glm::vec2 difference = std::get<2>(collision);
					if (dir == LEFT || dir == RIGHT)             // horizontial collision
					{
						Ball->Velocity.x  = -Ball->Velocity.x;
						float penetration = Ball->m_Raduis - std::abs(difference.x);
						if(dir == LEFT)
							Ball->Position.x += penetration;     // if face Left, then move to right 
						else
							Ball->Position.x -= penetration;
					}
					else                                         // vertial collision
					{
						Ball->Velocity.y  = -Ball->Velocity.y;
						float penetration = Ball->m_Raduis - std::abs(difference.y);
						if(dir == UP)
							Ball->Position.y -= penetration;     // if face Up, then move to down 
						else
							Ball->Position.y += penetration;
					}
				}
			}
		}

		Collision collision = CheckCollision(*Ball, *Player);
		if (!Ball->m_Stuck && std::get<0>(collision))
		{
			float paddleCenter = Player->Position.x + Player->Size.x / 2.0f;
			float ballCenter   = Ball->Position.x + Ball->m_Raduis;
			float distance     = ballCenter - paddleCenter;
			float percentage   = distance / (Player->Size.x / 2.0f);

			float strength     = 2.0f;
			float speed        = glm::length(Ball->Velocity);
			Ball->Velocity.x   = INITIAL_BALL_VELOCITY.x * percentage * strength;
			// due to the high speed paddle moving, this will make the ball stick 
			// because multi times collision dectected and filp its vertical velocity
			//Ball->Velocity.y   = -Ball->Velocity.y;
			Ball->Velocity.y   = -1 * std::abs(Ball->Velocity.y);
			Ball->Velocity     = glm::normalize(Ball->Velocity) * speed;
		}
	}

	void Game::ResetLevel()
	{
		for (auto& brick : m_Levels[m_CurrentLevel].Bricks)
		{
			brick.Destroyed = false;
		}
	}

	void Game::ResetPlayer()
	{
		glm::vec2 ballPos(m_Width / 2.0f - BALL_RADIUS, m_Height - PLAYER_SIZE.y - BALL_RADIUS - BALL_RADIUS);
		Ball->Position = ballPos;
		Ball->Velocity = INITIAL_BALL_VELOCITY;
		Ball->m_Stuck  = true;
		
		glm::vec2 playerPos((m_Width - PLAYER_SIZE.x) / 2.0f, m_Height - PLAYER_SIZE.y);
		Player->Position = playerPos;
	}

	// Calculates which direction a vector is facing (N,E,S or W)
	Direction VectorDirection(glm::vec2 circleToClosest)
	{
		glm::vec2 compass[4] = {
			glm::vec2( 0.0f,  1.0f),     // Up
			glm::vec2( 1.0f,  0.0f),     // Right
			glm::vec2( 0.0f, -1.0f),     // Down
			glm::vec2(-1.0f,  0.0f)      // Left
		};

		int direction = -1;
		float max = 0.0f;
		glm::vec2 vector = glm::normalize(circleToClosest);
		for (int i = 0; i < 4; i++)
		{
			float dot_product = glm::dot(vector, compass[i]);
			if (dot_product > max)
			{
				max = dot_product;
				direction = i;
			}
		}
		return (Direction)direction;
	}

	bool CheckCollision(GameObject& a, GameObject& b)
	{
		bool collisionX = a.Position.x + a.Size.x >= b.Position.x && b.Position.x + b.Size.x >= a.Position.x;
		bool collisionY = a.Position.y + a.Size.y >= b.Position.y && b.Position.y + b.Size.y >= a.Position.y;
		return collisionX && collisionY;
	}

	Collision CheckCollision(BallObject& ball, GameObject& b)
	{
		glm::vec2 center(ball.Position + ball.m_Raduis);

		glm::vec2 aabb_half_extents(b.Size.x / 2.0f, b.Size.y / 2.0f);
		glm::vec2 aabb_center(b.Position.x + aabb_half_extents.x, b.Position.y + aabb_half_extents.y);

		glm::vec2 distance(center - aabb_center);
		glm::vec2 clamped(glm::clamp(distance, -aabb_half_extents, aabb_half_extents));

		glm::vec2 closest(aabb_center + clamped);          // the closest point in AABB with circle center
		glm::vec2 circleToClosest = closest - center;
		if (glm::length(circleToClosest) < ball.m_Raduis)
		{
			return std::make_tuple(true, VectorDirection(circleToClosest), circleToClosest);
		}
		return std::make_tuple(false, UP, glm::vec2(0.0f, 0.0f));
	}


}
