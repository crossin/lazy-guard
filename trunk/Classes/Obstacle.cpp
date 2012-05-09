#include "Obstacle.h"
#include "Gameplay.h"
#include "PathFinder.h"

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
		fire = NULL;

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

void Obstacle::kill()
{
	if (fire)
	{
		fire->terrain = NULL;
		fire->kill();
	}

	Gameplay* gp = (Gameplay*)getParent();
	gp->obstacles->removeObject(this);
	gp->things->removeObject(this);
	PathFinder* pf = PathFinder::getInstance();
	pf->setUnwalkable((int)getPosition().x/pf->tileWidth, (int)getPosition().y/pf->tileHeight, false);
	removeFromParentAndCleanup(true);
}

