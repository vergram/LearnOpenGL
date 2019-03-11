#include "game_level.h"

#include <fstream>
#include <sstream>

#include "resource_manager.h"

namespace BreakOut {



	void GameLevel::Load(const char * file, unsigned int levelWidth, unsigned int levelHeight)
	{
		Bricks.clear();
		unsigned int tileCode;
		std::string line;
		std::ifstream fs(file);
		std::vector<std::vector<unsigned int>> tileData;
		if (fs)
		{
			while (std::getline(fs, line))
			{
				std::istringstream sstream(line);
				std::vector<unsigned int> row;
				while (sstream >> tileCode)
				{
					row.push_back(tileCode);
				}
				tileData.push_back(row);
			}
		}
		if (tileData.size() > 0)
		{
			init(tileData, levelWidth, levelHeight);
		}
	}

	void GameLevel::Draw(SpriteRenderer & renderer)
	{
		for (auto& brick : Bricks)
		{
			if(!brick.Destroyed)
				renderer.DrawSprite(brick.Sprite, brick.Position, brick.Size, brick.Rotation, brick.Color);
		}
	}

	bool GameLevel::IsComleted()
	{
		for (auto& brick : Bricks)
		{
			if (!brick.IsSolid && !brick.Destroyed) return false;
		}

		return true;
	}

	void GameLevel::init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight)
	{
		unsigned int width = tileData[0].size();
		unsigned int height = tileData.size();
		float unitWidth = (float)levelWidth / (float)width;
		float unitHeight = (float)levelHeight / (float)height;

		for (unsigned int y = 0; y < height; y++)
		{
			for (unsigned int x = 0; x < width; x++)
			{
				if (tileData[y][x] == 1)
				{
					glm::vec2 pos(unitWidth * x, unitHeight * y);
					glm::vec2 size(unitWidth, unitHeight);
					GameObject obj(pos, size,
						ResourceManager::GetTexture("block_solid"), 
						glm::vec3(0.8f, 0.8f, 0.7f));
					obj.IsSolid = true;
					Bricks.push_back(obj);
				}
				else if (tileData[y][x] > 1)
				{
					glm::vec3 color = glm::vec3(1.0f); // original: white
					if (tileData[y][x] == 2)
						color = glm::vec3(0.2f, 0.6f, 1.0f);
					else if (tileData[y][x] == 3)
						color = glm::vec3(0.0f, 0.7f, 0.0f);
					else if (tileData[y][x] == 4)
						color = glm::vec3(0.8f, 0.8f, 0.4f);
					else if (tileData[y][x] == 5)
						color = glm::vec3(1.0f, 0.5f, 0.0f);

					glm::vec2 pos(unitWidth * x, unitHeight * y);
					glm::vec2 size(unitWidth, unitHeight);

					GameObject obj(pos, size,
						ResourceManager::GetTexture("block"), color);
					Bricks.push_back(obj);
				}
			}
		}
	}

}