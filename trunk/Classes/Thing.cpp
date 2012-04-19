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
	CCRect box = sprite->boundingBox();
	return CCRectMake(getPosition().x + box.origin.x, getPosition().y + box.origin.y, box.size.width, box.size.height);
// 	return CCRectMake(getPosition().x - sprite->getContentSize().width * sprite->getAnchorPoint().x,
// 		getPosition().y - sprite->getContentSize().height * sprite->getAnchorPoint().y,
// 		sprite->getContentSize().width,
// 		sprite->getContentSize().height);
}

bool Eraser::init()
{
	bool bRet = false;
	do{
		sprite = CCSprite::spriteWithSpriteFrameName("eraser.png");
		sprite->setAnchorPoint(CCPointZero);
		addChild(sprite);

		bRet=true;
	}while(0);

	return bRet;
}

Eraser* Eraser::eraser()
{
	Eraser *eraser = new Eraser;

	if (eraser && eraser->init())
	{
		eraser->autorelease();
		return eraser;
	}

	return NULL;
}