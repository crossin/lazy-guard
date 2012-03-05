#include "Gem.h"

#include "Gameplay.h"
using namespace cocos2d;

Gem::Gem(void)
{
}


Gem::~Gem(void)
{
}

Gem* Gem::gem()
{
	Gem *gem = new Gem;

	if (gem && gem->init())
	{
		gem->autorelease();
		return gem;
	}

	return NULL;
}

bool Gem::init()
{
	bool bRet = false;
	do{
		this->setAnchorPoint(CCPointZero);

		sprite = CCSprite::spriteWithFile("gem.png");
		sprite->setAnchorPoint(CCPointZero);
		this->addChild(sprite);

		//owner = NULL;

		bRet=true;
	}while(0);
	
	return bRet;
}


void Gem::kill()
{
	//owner = NULL;
	removeFromParentAndCleanup(true);
}