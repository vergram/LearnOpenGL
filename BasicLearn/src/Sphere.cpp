#include "Sphere.h"
#include "Renderer.h"
#include "VertexBufferLayout.h"

///////////////////////////////////////////////////////////////////////////////
// build vertices of sphere with smooth shading using parametric equation. MORE MATH: http://www.scratchapixel.com/lessons/mathematics-physics-for-computer-graphics/mathematics-of-shading
// x = r * sin(θ) * cos(ϕ)
// z = r * sin(θ) * sin(ϕ)
// y = r * cos(θ)
// where θ: stack(latitude)   angle (0 <= θ <= π)
//       ϕ: sector(longitude) angle (0 <= ϕ <= 2π)
///////////////////////////////////////////////////////////////////////////////
Sphere::Sphere(int sectors, int stacks, float radius)
{
	const float PI = 3.1415926f;

	float x, y, z, sinTheta;                            // vertex position
	float nx, ny, nz, lengthInv = 1.0f / radius;        // normal
	float u, v;                                         // texCoord

	float sectorStep = 2.0f * PI / float(sectors);
	float stackStep = PI / float(stacks);
	float phi, theta;                                   // phi ∈ [0, 2π],  theta ∈ [0, π]

	for (int i = 0; i <= stacks; i++)
	{
		theta = stackStep * i;                          // θ starting form 0 to π
		sinTheta = radius * sinf(theta);                // r * sin(θ);
		y = radius * cosf(theta);                       // r * cos(θ);

		for (int j = 0; j <= sectors; j++)
		{
			phi = sectorStep * j;                       // starting form 0 to 2 * PI

			// vertex position
			x = sinTheta * cosf(phi);                   // x = r * sin(θ) * cos(ϕ)
			z = sinTheta * sinf(phi);                   // z = r * sin(θ) * sin(ϕ)
			positions.push_back(glm::vec3(x, y, z));

			// normalize vectex normal
			nx = x * lengthInv;
			ny = y * lengthInv;
			nz = z * lengthInv;
			normals.push_back(glm::vec3(nx, ny, nz));

			// vertex tex coord between [0, 1]
			u = float(j) / sectors;
			v = float(i) / stacks;
			texCoords.push_back(glm::vec2(u, v));
		}
	}

	// indices
	//  k1--k1+1
	//  |  / |
	//  | /  |
	//  k2--k2+1

	int k1, k2;
	for (int i = 0; i < stacks; i++)
	{
		k1 = i * (sectors + 1);                     // begining of current stack
		k2 = k1 + sectors + 1;                      // begining of next stack

		for (int j = 0; j < sectors; j++, k1++, k2++)
		{
			// 2 triangles per sector excluding 1st and last stacks
			if (i != 0)
			{
				indices.push_back(k1);         // k1 -- k2 -- k1+1
				indices.push_back(k2);
				indices.push_back(k1 + 1);
			}
			if (i != (stacks - 1))
			{
				indices.push_back(k1 + 1);     // k1+1 -- k2 -- k2+1
				indices.push_back(k2);
				indices.push_back(k2 + 1);
			}
		}
	}

	int positionSize = 3 * sizeof(float) * positions.size();

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float) * positions.size(), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, positionSize, positions.data());
	glBufferSubData(GL_ARRAY_BUFFER, positionSize, positionSize, normals.data());
	glBufferSubData(GL_ARRAY_BUFFER, 2 * positionSize, 2 * sizeof(float) * texCoords.size(), texCoords.data());

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, 3 * sizeof(float), (void*)(positionSize));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, false, 2 * sizeof(float), (void*)(positionSize + positionSize));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glGenBuffers(1, &m_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

Sphere::~Sphere()
{
	GLCall(glDeleteVertexArrays(1, &m_VAO));
}

void Sphere::Bind() const
{
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
}

void Sphere::UnBind() const
{
	glBindVertexArray(0);
}

