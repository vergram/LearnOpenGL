#include "game_object.h"

namespace BreakOut {

	GameObject::GameObject()
		:Position(0, 0), Size(1, 1), Velocity(0.0f), Color(1.0f), Rotation(0.0f), Sprite(), IsSolid(false), Destroyed(false)
	{}

	GameObject::GameObject(glm::vec2 position, glm::vec2 size, Texture2D & sprite, glm::vec3 color, glm::vec2 velocity)
		: Position(position), Size(size), Velocity(velocity), Color(color), Rotation(0.0f), Sprite(sprite), IsSolid(false), Destroyed(false)
	{}

	void GameObject::Draw(SpriteRenderer & render)
	{
		render.DrawSprite(Sprite, Position, Size, Rotation, Color);
	}

}
