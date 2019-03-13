#pragma once

#include "shader.h"
#include "texture.h"
#include "game_object.h"

#include <glm/ext.hpp>
#include <glm/glm.hpp>

namespace BreakOut {

	struct Particle
	{
		glm::vec2 Position, Velocity;
		glm::vec4 Color;
		float LifeTime;

		Particle():Position(0.0f), Velocity(0.0f), Color(1.0f), LifeTime(0.0f){}
	};

	class ParticleGenerator
	{
	public:
		ParticleGenerator(Shader& shader, Texture2D& texture, unsigned int amount);

		void Update(float deltaTime, GameObject& object, unsigned int newParticles, glm::vec2 offset = glm::vec2(0.0f, 0.0f));

		void Draw();
	private:
		std::vector<Particle> m_Particles;
		unsigned int m_Amount;

		Shader& m_Shader;
		Texture2D& m_Texture;
		unsigned int m_VAO;

		void init();

		unsigned int firstUnusedPartice();

		void respawnParticle(Particle& particle, GameObject& object, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
	};

}