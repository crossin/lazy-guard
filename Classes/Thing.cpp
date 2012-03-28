#include "Thing.h"
#include "cocos2d.h"

using namespace cocos2d;

Thing::Thing(void)
{
}

Thing::~Thing(void)
{
}

CCRect Thing::getRectOut()
{
	return CCRectMake(getPosition().x - sprite->getContentSize().width * sprite->getAnchorPoint().x,
		getPosition().y - sprite->getContentSize().height * sprite->getAnchorPoint().y,
		sprite->getContentSize().width,
		sprite->getContentSize().height);
}
