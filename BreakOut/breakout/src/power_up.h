#pragma once

#include "game_object.h"

namespace BreakOut {

	const glm::vec2 POWER_SIZE(60.0f, 20.0f);
	const glm::vec2 POWER_VELOCITY(0.0f, 150.0f);

	class PowerUp : public GameObject
	{
	public:
		// power state
		std::string Type;
		float Duration;
		bool Activated;

		PowerUp(std::string type, glm::vec3 color, float duration, glm::vec2 position, Texture2D texture)
			:GameObject(position, POWER_SIZE, texture, color, POWER_VELOCITY), Type(type), Duration(duration), Activated(false)
		{}
	};

}