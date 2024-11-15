#include "Block.h"
#include <memory>
#include <vector>


Block::Block(Vector2D position, Object::Type name, const std::unique_ptr<GUI>& gui)
	: Object(position, name, gui)
{

}

Object* Block::copyMe()
{
	return new Block(*this);
}


