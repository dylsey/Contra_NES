#include "Enemy.h"
#include "GUI.h"
#include "Player.h"


Enemy::Enemy(std::string animationFile, Vector2D position, const std::unique_ptr<GUI>& gui)
	: AnimatedObject(animationFile, position, Object::Type::enemy, gui)
{

}

void Enemy::update(Object::Command command, std::vector<std::unique_ptr<Object>>& objects)
{
	while (getName() == Object::Type::player)
	{
		if (position < getPosition())
		{
			state = State::stillRight;
		}
		else
		{
			state = State::stillLeft;
		}
		updateSprite();
	}
}

Object* Enemy::copyMe()
{
	return new Enemy(*this);
}
