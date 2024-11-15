#include "AnimatedObject.h"
#include "Object.h"
#include "Block.h"
#include "GUI.h"
#include <fstream>
#include <iostream>


const float AnimatedObject::deltaT{ 0.5f };
const Vector2D AnimatedObject::gravity{ 0.0f, 10.0f };

AnimatedObject::AnimatedObject(std::string animationFile, Vector2D columnRow, Type name, const std::unique_ptr<GUI>& gui)
	:Object(position, name, gui)
{
	std::fstream fin;
	fin.open(animationFile);
	if (!fin)
	{
		std::cout << "Error opening file" << std::endl;
		return;
	}

	int numberOfStates;

	fin >> numberOfStates;
	for (int i{ 0 }; i < numberOfStates; i++)
	{
		int animationsInState;
		fin >> animationsInState;
		for (int j{ 0 }; j < animationsInState; j++)
		{
			//pushes back sprites on the vector according to the size of the animation
			int spriteSequence;
			fin >> spriteSequence;
			sprites[(State)i].push_back(spriteSequence);
		}
	}
	fin.close();
	updateSprite();
}

int AnimatedObject::getCurrentSprite() const
{
	return currentSprite;
}

void AnimatedObject::updateSprite()
{
	currentAnimation++;
	currentAnimation %= sprites[state].size();
	currentSprite = sprites[state][currentAnimation];
}

void AnimatedObject::doPhysics(const std::vector<std::unique_ptr<Object>>& objects)
{
	applyGravity();

	for (auto& object : objects)
	{
		switch (object->getName())
		{
		case Type::bridge_center:
		case Type::bridge_end:
		case Type::bridge_start:
		case Type::top_block:
		case Type::water_top:
		{
			if (collision(object))
			{

				position.y = object->getPosition().y - gui->getDimensions(this).y;
				velocity.y = 0;
				if (state == AnimatedObject::State::jumpLeft)
				{
					state = AnimatedObject::State::stillLeft;
				}
				else if (state == AnimatedObject::State::jumpRight)
				{
					state = AnimatedObject::State::stillRight;
				}
			}
			break; //breaks out of the switch statement
		}
		default:
			break;
		}
	}
}

void AnimatedObject::applyGravity()
{
	velocity.y += gravity.y * deltaT;
	position.y += velocity.y * deltaT;
}

bool AnimatedObject::collision(const std::unique_ptr<Object>& object, bool full)
{
	//boundaries of the *this object/animated object
	float animatedObjectLeft = position.x;
	float animatedObjectRight = position.x + getDimensions().x;
	float animatedObjectTop = position.y;
	float animatedObjectBottom = position.y + getDimensions().y;

	//boundries of the object we are colliding against
	float collisionObjectLeft = object->getPosition().x;
	float collisionObjectRight = object->getPosition().x + object->getDimensions().x;
	float collisionObjectTop = object->getPosition().y;
	float collisionObjectBottom = object->getPosition().y + object->getDimensions().y;

	//collision with enemy happend when full is true->use previous collision detection
	if (full)
	{
		bool leftInColumn{ (animatedObjectRight >= collisionObjectLeft) && (animatedObjectLeft <= collisionObjectRight) };
		bool rightInColumn{ (animatedObjectRight >= collisionObjectLeft) && (animatedObjectRight <= collisionObjectRight) };
		bool topInRow{ (animatedObjectBottom >= collisionObjectTop) && (animatedObjectTop <= collisionObjectBottom) };
		bool bottomInRow{ (animatedObjectBottom >= collisionObjectTop) && (animatedObjectBottom <= collisionObjectBottom) };
		bool objectTopInThis{ (collisionObjectTop >= animatedObjectTop) && (collisionObjectTop <= animatedObjectBottom) };

		bool sameColumn{ leftInColumn || rightInColumn };
		bool sameRow{ topInRow || bottomInRow || objectTopInThis };

		return sameRow && sameColumn;
	}
	else
	{
		bool leftInColumn{ (animatedObjectRight >= collisionObjectLeft) && (animatedObjectLeft <= collisionObjectRight) };
		bool rightInColumn{ (animatedObjectRight >= collisionObjectLeft) && (animatedObjectRight <= collisionObjectRight) };
		/*bool topInRow{ (animatedObjectBottom >= collisionObjectTop) && (animatedObjectTop <= collisionObjectBottom) };*/
		bool bottomInRow{ (animatedObjectBottom >= collisionObjectTop) && (animatedObjectBottom <= collisionObjectBottom) };
		bool objectTopInThis{ (collisionObjectTop >= animatedObjectTop) && (collisionObjectTop <= animatedObjectBottom) };

		bool sameColumn{ leftInColumn || rightInColumn };
		//we dont check for topInRow if there is not an enemy to collide with. that way we can plaform! 
		bool sameRow{ bottomInRow || objectTopInThis };

		return sameRow && sameColumn;
	}

}

