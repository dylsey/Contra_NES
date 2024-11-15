#include <fstream>
#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>

#include "Player.h"
#include "GUI.h"
#include "Block.h"
#include "Object.h"
#include "engine.h"

Player::Player(std::string animationFile, Vector2D position, const std::unique_ptr<GUI>& gui)
	: AnimatedObject(animationFile, position, Object::Type::player, gui)
{
}

//Don't need to pass in const Player& player1 since we are passing in a uniuqe vector of object pointers
void Player::update(Object::Command command, std::vector<std::unique_ptr<Object>>& objects)
{
	switch (command)
	{
	case Object::Command::right:moveRight(); break;
	case Object::Command::left:moveLeft(); break;
	case Object::Command::down:moveDown(); break;
	case Object::Command::up:moveUp(); break;
	case Object::Command::jump:moveJump(); break;
	case Object::Command::NA:noAction(); break;
	}

	doPhysics(objects);

	//sprite update and height adjustment
	int tempHeight{ (int)gui->getDimensions(this).y };
	updateSprite();

	position.y -= gui->getDimensions(this).y - tempHeight;
	if (gui->getDimensions(this).y >= GUI::screenDimensions.y)
	{
		isDead = true; 
	}
	//border detection.
	if (position.x < 0)
	{
		position.x = 0;
	}
	else if (position.x > gui->screenDimensions.x - gui->getDimensions(this).x)
	{
		position.x = gui->screenDimensions.x - gui->getDimensions(this).x;
	}
	
	
	if (position.x >= GUI::screenDimensions.x / 2)
	{
		if (!lastBlockOnScreen(objects))
		{
			//Int that is the difference between the player and the object at the edge of the screen
			int playerScreenDifference{ position.x - GUI::screenDimensions.x / 2 };
			//Supposed to shift all of the objects with the differnce between the player and the screen
			//Important note, the screen isn't panning, all of the objects in the world are shifting with the player
			for (auto& object : objects)
			{
				object->setPosition({ object->getPosition().x - playerScreenDifference, object->getPosition().y });
			}
		}
	}
		
	//Code for player/enemy collision
	for (const auto& object : objects)
	{
		if (object->getName() == Object::Type::enemy && AnimatedObject::collision(object, true))
		{
			isDead = true;
		}
	}
}

Object* Player::copyMe()
{
	return new Player(*this);
}

bool Player::getIsDead()
{
	return isDead;
}

bool Player::lastBlockOnScreen(const std::vector<std::unique_ptr<Object>>& objects)
{
	//This loops over the objects vector and finds the object with the largest x value (being sure not to check the player). 
	//It returns true if that value is less than or equal to GUI::screenDimensions.x otherwise false.

	//int largestX{ 0 };

	//Nice job optomizing this, looks cleaner than what I had
	for (auto& object : objects)
	{
		if ((object->getPosition().x + object->getDimensions().x) > GUI::screenDimensions.x)
		{
			return false;
		}
		//I originally included else {false} here, but I realized it just immedietly exited the for loop, so it is now adjusted to be outside of the loop
	}
	return true;
}

void Player::moveRight()
{
	switch (state)
	{
	case State::stillLeft:
		state = State::stillRight;
		break;
	case State::stillRight:
		state = State::walkRight;
		break;
	case State::walkRight:
		position.x += walkSpeed;
		break;

	}
}

void Player::moveLeft()
{
	switch (state)
	{
	case AnimatedObject::State::stillRight:
		state = AnimatedObject::State::stillLeft;
		break;
	case AnimatedObject::State::stillLeft:
		state = AnimatedObject::State::walkLeft;
		break;
	case AnimatedObject::State::walkLeft:
		position.x -= walkSpeed;
		break;
	default:
		break;
	}
}

void Player::moveDown()
{
	switch (state)
	{
	case AnimatedObject::State::stillLeft:
		state = AnimatedObject::State::crouchLeft;
		break;
	case AnimatedObject::State::stillRight:
		state = AnimatedObject::State::crouchRight;
		break;
	}
}

void Player::moveJump()
{
	if (velocity.y == 0)
	{
		switch (state)
		{
		case Player::State::stillRight:
			state = State::jumpRight;
			velocity.y = -jumpStrength;
			break;
		case Player::State::stillLeft:
			state = State::jumpLeft;
			velocity.y = -jumpStrength;
			break;
		case Player::State::jumpRight:
			break;
		case Player::State::jumpLeft:
			break;
		default:
			break;
		}

	}
}

void Player::moveUp()
{
	switch (state)
	{
	case AnimatedObject::State::stillLeft:
		state = AnimatedObject::State::upLeft;
		break;
	case AnimatedObject::State::stillRight:
		state = AnimatedObject::State::upRight;
		break;
	}
}

void Player::noAction()
{
	switch (state)
	{
	case AnimatedObject::State::walkLeft:
	case AnimatedObject::State::crouchLeft:
	case AnimatedObject::State::upLeft:
		state = AnimatedObject::State::stillLeft;
		break;
	case AnimatedObject::State::walkRight:
	case AnimatedObject::State::crouchRight:
	case AnimatedObject::State::upRight:
		state = AnimatedObject::State::stillRight;
		break;
	}
}






