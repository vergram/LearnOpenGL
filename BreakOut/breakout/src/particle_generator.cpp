#include "particle_generator.h"

namespace BreakOut {

	

	ParticleGenerator::ParticleGenerator(Shader& shader, Texture2D& texture, unsigned int amount)
		:m_Shader(shader), m_Texture(texture), m_Amount(amount)
	{
		init();
	}

	void ParticleGenerator::Update(float deltaTime, GameObject & object, unsigned int newParticles, glm::vec2 offset)
	{
		// add new Particle
		for (int i = 0; i < newParticles; i++)
		{
			unsigned int unusedIndex = firstUnusedPartice();
			respawnParticle(m_Particles[unusedIndex], object, offset);
		}

		// update all particles
		for (int i = 0; i < m_Amount; i++)
		{
			Particle& p = m_Particles[i];
			p.LifeTime -= deltaTime;
			if (p.LifeTime > 0.0f)
			{
				p.Position -= p.Velocity * deltaTime;
				p.Color.a -= deltaTime * 2.5f;
			}
		}
	}

	void ParticleGenerator::Draw()
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		m_Shader.Bind();
		m_Texture.Bind();
		for (auto& p : m_Particles)
		{
			if (p.LifeTime > 0.0f)
			{
				m_Shader.SetUniform2f("offset", p.Position);
				m_Shader.SetUniform4f("color", p.Color);
				glBindVertexArray(m_VAO);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
		}
		glBindVertexArray(0);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void ParticleGenerator::init()
	{
		// Set up mesh and attribute properties
		unsigned int VBO;
		float particle_quad[] = {
			0.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,

			0.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 0.0f
		};
		glGenVertexArrays(1, &m_VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(m_VAO);
		// Fill mesh buffer
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
		// Set mesh attributes
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
		glBindVertexArray(0);

		for (unsigned int i = 0; i < m_Amount; i++)
		{
			m_Particles.push_back(Particle());
		}
	}

	// Stores the index of the last particle used (for quick access to next dead particle)
	unsigned int lastUsedParticle = 0;
	unsigned int ParticleGenerator::firstUnusedPartice()
	{
		// First search from last used particle, this will usually return almost instantly
		for (unsigned int i = lastUsedParticle; i < m_Amount; i++)
		{
			if (m_Particles[i].LifeTime <= 0.0f)
			{
				lastUsedParticle = i;
				return i;
			}
		}

		// Otherwise, do a linear search
		for (unsigned int i = 0; i < lastUsedParticle; i++)
		{
			if (m_Particles[i].LifeTime <= 0.0f)
			{
				lastUsedParticle = i;
				return i;
			}
		}
		// All particles are taken, override the first one (note that if it repeatedly hits this case, more particles should be reserved)
		lastUsedParticle = 0;
		return 0;
	}

	void ParticleGenerator::respawnParticle(Particle & particle, GameObject & object, glm::vec2 offset)
	{
		float random = (rand() % 100 - 50) / 10.0f;
		float rColor = 0.5f + (rand() % 100) / 100.0f;
		particle.Position = object.Position + offset + random;
		particle.LifeTime = 1.0f;
		particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
		particle.Velocity = object.Velocity * 0.1f;
	}

}