#include "Obstacle.h"

using namespace cocos2d;

Obstacle::Obstacle(void)
{
}


Obstacle::~Obstacle(void)
{
}

Obstacle* Obstacle::obstacle(int t, CCPoint pos)
{
	Obstacle *obstacle = new Obstacle;

	if (obstacle && obstacle->init(t, pos))
	{
		obstacle->autorelease();
		return obstacle;
	}

	return NULL;
}

bool Obstacle::init()
{
	bool bRet = false;
	do{
		//this->setAnchorPoint(CCPointZero);

		switch (typeIndex)
		{
		case 1:
			sprite = CCSprite::spriteWithSpriteFrameName("box.png");
			break;
		case 2:
			sprite = CCSprite::spriteWithSpriteFrameName("brick.png");
			break;
		case 3:
			sprite = CCSprite::spriteWithSpriteFrameName("house.png");
			break;
		case 4:
		default:
			sprite = CCSprite::spriteWithSpriteFrameName("tree.png");
			break;
		}
		addChild(sprite);

		bRet=true;
	}while(0);
	
	return bRet;
}

bool Obstacle::init(int t, CCPoint pos)
{
	setPosition(pos);
	typeIndex = t;
	return init();
}
// void Obstacle::kill()
// {
// 	//owner = NULL;
// 	removeFromParentAndCleanup(true);
// }

