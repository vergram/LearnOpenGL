#pragma once

#include "game_object.h"

namespace BreakOut {

	class BallObject :public GameObject
	{
	public: 
		float m_Raduis;
		bool m_Stuck;

		BallObject();
		BallObject(glm::vec2 pos, float radius, glm::vec2 velocity, Texture2D sprite);

		glm::vec2 Move(float deltaTime, unsigned int windowWidth);
		void Reset(glm::vec2 position, glm::vec2 velocity);
	};

}