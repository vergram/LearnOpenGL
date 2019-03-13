#include "ball_object_collisions.h"

namespace BreakOut {

	BallObject::BallObject()
		:GameObject(), m_Raduis(12.5f), m_Stuck(true)
	{}

	BallObject::BallObject(glm::vec2 pos, float radius, glm::vec2 velocity, Texture2D sprite)
		:GameObject(pos, glm::vec2(radius * 2.0f, radius * 2.0f), sprite, glm::vec3(1.0f), velocity),
		m_Raduis(radius), m_Stuck(true), Sticky(false), PassThrough(false)
	{}

	glm::vec2 BallObject::Move(float deltaTime, unsigned int windowWidth)
	{
		if (!m_Stuck)
		{
			Position += Velocity * deltaTime;
			if (Position.x <= 0.0f)
			{
				Velocity.x = -Velocity.x;
				Position.x = 0.0f;
			}
			else if (Position.x >= windowWidth - Size.x)
			{
				Velocity.x = -Velocity.x;
				Position.x = windowWidth - Size.x;
			}
			if (Position.y <= 0.0f)
			{
				Velocity.y = -Velocity.y;
				Position.y = 0.0f;
			}
		}

		return Position;
	}

	void BallObject::Reset(glm::vec2 position, glm::vec2 velocity)
	{
		Position = position;
		Velocity = velocity;
		m_Stuck = true;
		Sticky = false;
		PassThrough = false;
	}

}