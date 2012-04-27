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
		switch (typeIndex)
		{
		case TREE:
		default:
			sprite = CCSprite::spriteWithSpriteFrameName("tree.png");
			break;
		case WOOD:
			sprite = CCSprite::spriteWithSpriteFrameName("wood.png");
			break;
		case ROCK:
			sprite = CCSprite::spriteWithSpriteFrameName("rock.png");
			break;
		case STEEL:
			sprite = CCSprite::spriteWithSpriteFrameName("steel.png");
			break;
		}
		sprite->setAnchorPoint(CCPointZero);
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

