#include "Fire.h"
#include "AnimatePacker.h"

Fire::Fire(void)
{
}

Fire::~Fire(void)
{
}

Fire* Fire::fire()
{
	Fire *fire = new Fire;

	if (fire && fire->init())
	{
		fire->autorelease();
		return fire;
	}

	return NULL;
}

bool Fire::init()
{
	bool bRet = false;
	do{
		sprite = CCSprite::spriteWithSpriteFrameName("fire1.png");
		sprite->setPosition(ccp(0, 20));
		addChild(sprite);
		owner = NULL;
		terrain = NULL;
		lifetime = 5;
		scheduleUpdate();
		CCRepeatForever* actBurn = CCRepeatForever::actionWithAction(AnimatePacker::getInstance()->getAnimate("fire"));
		sprite->runAction(actBurn);

		bRet=true;
	}while(0);

	return bRet;
}

void Fire::kill()
{
	if (owner)
	{
		owner->setFire(false);
	}
	removeFromParentAndCleanup(true);
}

void Fire::update(ccTime dt)
{
	lifetime -= dt;
	if (lifetime < 0)
	{
		kill();
	}
	if (owner)
	{
		setPosition(owner->getPosition());
	}
}