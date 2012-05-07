#include "Bomb.h"


Bomb::Bomb(void)
{
}

Bomb::~Bomb(void)
{
}

Bomb* Bomb::bomb()
{
	Bomb *bomb = new Bomb;

	if (bomb && bomb->init())
	{
		bomb->autorelease();
		return bomb;
	}

	return NULL;
}

bool Bomb::init()
{
	bool bRet = false;
	do{
		sprite = CCSprite::spriteWithSpriteFrameName("bomb.png");
		//sprite->setPosition(ccp(0, 20));
		addChild(sprite);
		timeLife = 5;
		scheduleUpdate();

		bRet=true;
	}while(0);

	return bRet;
}

void Bomb::kill()
{
	removeFromParentAndCleanup(true);
}

void Bomb::update(ccTime dt)
{
	timeLife -= dt;
	if (timeLife < 0)
	{
		kill();
	}
}