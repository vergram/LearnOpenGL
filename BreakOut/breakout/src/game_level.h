#pragma once

#include "game_object.h"

namespace BreakOut {

	class GameLevel
	{
	public:
		std::vector<GameObject> Bricks;

		GameLevel(){}

		void Load(const char* file, unsigned int levelWidth, unsigned int levelHeight);
		void Draw(SpriteRenderer& renderer);
		bool IsComleted();

	private:
		void init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight);
	};

}