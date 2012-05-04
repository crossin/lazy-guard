#include "Fire.h"
#include "AnimatePacker.h"
#include "Gameplay.h"
#include "PathFinder.h"

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
		INTERVAL = 1;
		spreadInterval = INTERVAL;
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
	if (terrain)
	{
		terrain->kill();
	}
	removeFromParentAndCleanup(true);
}

void Fire::update(ccTime dt)
{
	lifetime -= dt;
	if (lifetime < 0)
	{
		kill();
		return;
	}
	if (owner)
	{
		setPosition(owner->getPosition());
	}

	// spread
	spreadInterval -= dt;
	if (spreadInterval < 0)
	{
		spreadInterval = INTERVAL;
		Gameplay* gp = (Gameplay*)getParent();
		//obstacle
		CCMutableArray<Obstacle*>::CCMutableArrayIterator iobs;
		Obstacle* obs;
		for (iobs = gp->obstacles->begin(); iobs != gp->obstacles->end(); iobs++)
		{
			obs = *iobs;
			if((obs->typeIndex==0 || obs->typeIndex==1) && !obs->onFire 
				&& CCRect::CCRectContainsPoint(obs->getRectOut(), ccpAdd(getPosition(), sprite->getPosition()))
				&& CCRANDOM_0_1() < 0.5)
			{
				int tWidth = PathFinder::getInstance()->tileWidth;
				Fire* fr = Fire::fire();
				fr->terrain = obs;
				obs->onFire = true;
				gp->addChild(fr, 900);
				fr->setPosition(ccpAdd(obs->getPosition(), ccp(tWidth/2,0)));
				return;
			}
		}
	}
	
	/*
	// guard
	CCMutableArray<Guard*>::CCMutableArrayIterator igd;
	Guard* gd;
	for (igd = guards->begin(); igd != guards->end(); igd++)
	{
		gd = *igd;
		if( CCRect::CCRectContainsPoint(gd->getRectOut(), posTouch) && !gd->inAction){
			Fire* fr = Fire::fire();
			fr->owner = gd;
			addChild(fr, 900);
			gd->setFire(true);
			toolSelected->unselected();
			toolSelected = NULL;
			return;
		}
	}
	// thief
	CCMutableArray<Thief*>::CCMutableArrayIterator itf;
	Thief* tf;
	for (itf = thieves->begin(); itf != thieves->end(); itf++)
	{
		tf = *itf;
		if( CCRect::CCRectContainsPoint(tf->getRectOut(), posTouch) && !tf->inAction){
			Fire* fr = Fire::fire();
			fr->owner = tf;
			addChild(fr, 900);
			if (tf->gem)
			{
				reorderChild(tf->gem, 0);
				tf->gem->setPosition(tf->getPosition());
				gemsOutside->addObject(tf->gem);
				tf->gem = NULL;
			}
			tf->setFire(true);
			//tf->clock = clk;
			toolSelected->unselected();
			toolSelected = NULL;
			return;
		}
	}*/
}