#pragma once

#include <glad/glad.h>

#include "game_level.h"
#include "power_up.h"

namespace BreakOut {

	enum GameState
	{
		GAME_ACTIVE,
		GAME_MENU,
		GAME_WIN
	};

	enum Direction
	{
		UP = 0, RIGHT, DOWN, LEFT
	};

	typedef std::tuple<bool, Direction, glm::vec2> Collision;
	
	class Game
	{
	public:

		Game(GLuint width, GLuint height);
		virtual ~Game();

		void Init();
		void DoCollisions();
		void ResetLevel();
		void ResetPlayer();

		void ProcessInput(GLfloat deltaTime);
		void OnUpdate(GLfloat deltaTime);
		void Render();

		inline GLuint GetWidth() const { return m_Width; }
		inline GLuint GetHeight() const { return m_Height; }
		inline GameState GetGameState() const { return m_State; }
		inline void SetGameState(GameState state) { m_State = state; }
		inline GLboolean GetKeyState(unsigned int keycode) { return m_Keys[keycode]; }
		inline void SetKeyState(unsigned int keycode, GLboolean isPressed) { m_Keys[keycode] = isPressed; }
	private:
		GameState m_State;
		GLboolean m_Keys[1024];
		GLuint m_Width, m_Height;

		std::vector<GameLevel> m_Levels;
		unsigned int m_CurrentLevel;

		std::vector<PowerUp> m_PowerUps;

		void SpawnPowerUps(GameObject& block);
		void UpdatePowerUps(float deltaTime);
	};

}