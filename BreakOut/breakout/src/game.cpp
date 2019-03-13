#include "game.h"

#include "resource_manager.h"
#include "SpriteRenderer.h"
#include "ball_object_collisions.h"
#include "particle_generator.h"
#include "post_processor.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <GLFW/glfw3.h>

#include <algorithm>

namespace BreakOut {

	BallObject* Ball;
	GameObject* Player;
	SpriteRenderer* Renderer;
	ParticleGenerator* Particles;
	PostProcessor* Effects;

	float ShakeTime = 0.0f;

	const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
	const float BALL_RADIUS = 12.5f;

	const glm::vec2 PLAYER_SIZE(100, 20);
	const float PLAYER_VELOCITY(500.0f);

	Game::Game(GLuint width, GLuint height)
		:m_State(GAME_ACTIVE),
		m_Keys(),
		m_Width(width),
		m_Height(height)
	{}

	Game::~Game()
	{
		delete Ball;
		delete Player;
		delete Renderer;
		delete Particles;
		delete Effects;
	}

	void Game::Init()
	{
		glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(m_Width), static_cast<float>(m_Height), 0.0f, -1.0f, 1.0f);

		// load Textures
		ResourceManager::LoadTexture("breakout/res/image/awesomeface.png"  , "ball");
		ResourceManager::LoadTexture("breakout/res/image/block.png"        , "block");
		ResourceManager::LoadTexture("breakout/res/image/block_solid.png"  , "block_solid");
		ResourceManager::LoadTexture("breakout/res/image/background.jpg"   , "background");
		ResourceManager::LoadTexture("breakout/res/image/paddle.png"       , "player");
		ResourceManager::LoadTexture("breakout/res/image/particle.png"     , "particle");

		ResourceManager::LoadTexture("breakout/res/image/powerup_passthrough.png", "powerup_passthrough");
		ResourceManager::LoadTexture("breakout/res/image/powerup_increase.png"   , "powerup_increase");
		ResourceManager::LoadTexture("breakout/res/image/powerup_confuse.png"    , "powerup_confuse");
		ResourceManager::LoadTexture("breakout/res/image/powerup_sticky.png"     , "powerup_sticky");
		ResourceManager::LoadTexture("breakout/res/image/powerup_chaos.png"      , "powerup_chaos");
		ResourceManager::LoadTexture("breakout/res/image/powerup_speed.png"      , "powerup_speed");

		// load shaders
		ResourceManager::LoadShader("breakout/res/shader/BasicSprite.shader", "sprite");
		ResourceManager::LoadShader("breakout/res/shader/ParticleShader.shader", "particle");
		ResourceManager::LoadShader("breakout/res/shader/PostProcessing.shader", "effect");

		// shader set up
		Shader& shader = ResourceManager::GetShader("sprite");
		shader.Bind();
		shader.SetUniform1i("image", 0);
		shader.SetUniformMatrix4fv("projection", projection);
		Renderer = new SpriteRenderer(shader);

		// particle
		Shader& particleShader = ResourceManager::GetShader("particle");
		particleShader.Bind();
		particleShader.SetUniform1i("sprite", 0);
		particleShader.SetUniformMatrix4fv("projection", projection);
		Particles = new ParticleGenerator(particleShader, ResourceManager::GetTexture("particle"), 200);

		// Effect
		Shader& effect = ResourceManager::GetShader("effect");
		Effects = new PostProcessor(effect, m_Width, m_Height);

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
		Particles->Update(deltaTime, *Ball, 2, glm::vec2(Ball->m_Raduis / 2.0f));
		if (ShakeTime > 0.0f)
		{
			ShakeTime -= deltaTime;
			if (ShakeTime <= 0.0f)
				Effects->Shake = false;
		}

		if (Ball->Position.y >= m_Height)
		{
			ResetLevel();
			ResetPlayer();
		}
		UpdatePowerUps(deltaTime);
	}

	void Game::Render()
	{
		if (m_State == GAME_ACTIVE)
		{
			Effects->BeginRender();
				Renderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0, 0), glm::vec2(m_Width, m_Height), 0.0f, glm::vec3(1.0f));
				m_Levels[m_CurrentLevel].Draw(*Renderer);
				Player->Draw(*Renderer);
				Ball->Draw(*Renderer);
				Particles->Draw();
				for (auto& power : m_PowerUps)
					if (!power.Destroyed)
						power.Draw(*Renderer);
			Effects->EndRender();

			Effects->Render(glfwGetTime());
		}
	}

	bool ShouldSpawn(unsigned int chance)
	{
		unsigned int random = rand() % chance;
		return random == 0;
	}

	void Game::SpawnPowerUps(GameObject & block)
	{
		if (ShouldSpawn(75)) // 1 in 75 chance
			this->m_PowerUps.push_back(
				PowerUp("speed", glm::vec3(0.5f, 0.5f, 1.0f), 0.0f, block.Position, ResourceManager::GetTexture("powerup_speed")
				));
		if (ShouldSpawn(75))
			this->m_PowerUps.push_back(
				PowerUp("sticky", glm::vec3(1.0f, 0.5f, 1.0f), 20.0f, block.Position, ResourceManager::GetTexture("powerup_sticky")
				));
		if (ShouldSpawn(75))
			this->m_PowerUps.push_back(
				PowerUp("pass-through", glm::vec3(0.5f, 1.0f, 0.5f), 10.0f, block.Position, ResourceManager::GetTexture("powerup_through")
				));
		if (ShouldSpawn(75))
			this->m_PowerUps.push_back(
				PowerUp("pad-size-increase", glm::vec3(1.0f, 0.6f, 0.4), 0.0f, block.Position, ResourceManager::GetTexture("powerup_increase")
				));
		if (ShouldSpawn(15)) // Negative m_PowerUps should spawn more often
			this->m_PowerUps.push_back(
				PowerUp("confuse", glm::vec3(1.0f, 0.3f, 0.3f), 15.0f, block.Position, ResourceManager::GetTexture("powerup_confuse")
				));
		if (ShouldSpawn(15))
			this->m_PowerUps.push_back(
				PowerUp("chaos", glm::vec3(0.9f, 0.25f, 0.25f), 15.0f, block.Position, ResourceManager::GetTexture("powerup_chaos")
				));
	}

	bool IsOtherPowerUpActive(std::vector<PowerUp> &powerUps, std::string type)
	{
		for (const PowerUp &powerUp : powerUps)
		{
			if (powerUp.Activated)
				if (powerUp.Type == type)
					return true;
		}
		return false;
	}

	void Game::UpdatePowerUps(float deltaTime)
	{
		for (auto& power : m_PowerUps)
		{
			power.Position += power.Velocity * deltaTime;
			if (power.Activated)
			{
				power.Duration -= deltaTime;
				if (power.Duration <= 0.0f)
				{
					power.Activated = false;

					if (power.Type == "sticky")
					{
						if (!IsOtherPowerUpActive(this->m_PowerUps, "sticky"))
						{	// Only reset if no other PowerUp of type sticky is active
							Ball->Sticky = GL_FALSE;
							Player->Color = glm::vec3(1.0f);
						}
					}
					else if (power.Type == "pass-through")
					{
						if (!IsOtherPowerUpActive(this->m_PowerUps, "pass-through"))
						{	// Only reset if no other PowerUp of type pass-through is active
							Ball->PassThrough = GL_FALSE;
							Ball->Color = glm::vec3(1.0f);
						}
					}
					else if (power.Type == "confuse")
					{
						if (!IsOtherPowerUpActive(this->m_PowerUps, "confuse"))
						{	// Only reset if no other PowerUp of type confuse is active
							Effects->Confuse = GL_FALSE;
						}
					}
					else if (power.Type == "chaos")
					{
						if (!IsOtherPowerUpActive(this->m_PowerUps, "chaos"))
						{	// Only reset if no other PowerUp of type chaos is active
							Effects->Chaos = GL_FALSE;
						}
					}
				}
			}
		}
		m_PowerUps.erase(std::remove_if(m_PowerUps.begin(), m_PowerUps.end(), 
			[](const PowerUp& power) { return !power.Activated && power.Destroyed; }
		), m_PowerUps.end());

	}

	void ActivatePowerUp(PowerUp &powerUp)
	{
		// Initiate a powerup based type of powerup
		if (powerUp.Type == "speed")
		{
			Ball->Velocity *= 1.2;
		}
		else if (powerUp.Type == "sticky")
		{
			Ball->Sticky = GL_TRUE;
			Player->Color = glm::vec3(1.0f, 0.5f, 1.0f);
		}
		else if (powerUp.Type == "pass-through")
		{
			Ball->PassThrough = GL_TRUE;
			Ball->Color = glm::vec3(1.0f, 0.5f, 0.5f);
		}
		else if (powerUp.Type == "pad-size-increase")
		{
			Player->Size.x += 50;
		}
		else if (powerUp.Type == "confuse")
		{
			if (!Effects->Chaos)
				Effects->Confuse = GL_TRUE; // Only activate if chaos wasn't already active
		}
		else if (powerUp.Type == "chaos")
		{
			if (!Effects->Confuse)
				Effects->Chaos = GL_TRUE;
		}
	}

	Direction VectorDirection(glm::vec2 circleToClosest);
	bool CheckCollision(GameObject& a, GameObject& b);
	Collision CheckCollision(BallObject& ball, GameObject& b);

	void Game::DoCollisions()
	{
		// box with ball
		for (GameObject& box : m_Levels[m_CurrentLevel].Bricks)
		{
			if (!box.Destroyed)
			{
				Collision collision = CheckCollision(*Ball, box);
				if (std::get<0>(collision))                      // is collision?
				{
					if (!box.IsSolid)
					{
						box.Destroyed = true;
						SpawnPowerUps(box);
					}
					else
					{
						ShakeTime = 0.05f;
						Effects->Shake = true;
					}
					Direction dir = std::get<1>(collision);
					glm::vec2 difference = std::get<2>(collision);
					if (!(Ball->PassThrough && !box.IsSolid)) // don't do collision resolution on non-solid bricks if pass-through activated
					{
						if (dir == LEFT || dir == RIGHT)             // horizontial collision
						{
							Ball->Velocity.x = -Ball->Velocity.x;
							float penetration = Ball->m_Raduis - std::abs(difference.x);
							if (dir == LEFT)
								Ball->Position.x += penetration;     // if face Left, then move to right 
							else
								Ball->Position.x -= penetration;
						}
						else                                         // vertial collision
						{
							Ball->Velocity.y = -Ball->Velocity.y;
							float penetration = Ball->m_Raduis - std::abs(difference.y);
							if (dir == UP)
								Ball->Position.y -= penetration;     // if face Up, then move to down 
							else
								Ball->Position.y += penetration;
						}
					}
				}
			}
		}

		// ball with player
		Collision collision = CheckCollision(*Ball, *Player);
		if (!Ball->m_Stuck && std::get<0>(collision))
		{
			Ball->m_Stuck      = Ball->Sticky;
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

		// powerup with player
		for (auto& power : m_PowerUps)
		{
			if (!power.Destroyed)
			{
				if (power.Destroyed >= m_Height)
				{
					power.Destroyed = true;
				}
				if (CheckCollision(*Player, power))
				{
					ActivatePowerUp(power);
					power.Activated = true;
					power.Destroyed = true;
				}
			}
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
