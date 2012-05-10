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
		timeLife = 5;
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
		owner->setFire(NULL);
	}
	if (terrain)
	{
		terrain->fire = NULL;
		terrain->kill();
	}
	removeFromParentAndCleanup(true);
}

void Fire::update(ccTime dt)
{
	if (owner)
	{
		setPosition(owner->getPosition());
	}
	timeLife -= dt;
	if (timeLife < 0)
	{
		kill();
		return;
	}

	// spread
	spreadInterval -= dt;
	if (spreadInterval < 0)
	{
		spreadInterval = INTERVAL;
		Gameplay* gp = (Gameplay*)getParent();
		CCPoint posFire = ccp(getPosition().x, getPosition().y+10);
		//obstacle
		CCMutableArray<Obstacle*>::CCMutableArrayIterator iobs;
		Obstacle* obs;
		for (iobs = gp->obstacles->begin(); iobs != gp->obstacles->end(); iobs++)
		{
			obs = *iobs;
			if((obs->typeIndex==0 || obs->typeIndex==1) && !obs->fire 
				&& CCRect::CCRectIntersectsRect(obs->getRectOut(), getRectIn())
				&& CCRANDOM_0_1() < 0.5)
			{
				int tWidth = PathFinder::getInstance()->tileWidth;
				Fire* fr = Fire::fire();
				fr->terrain = obs;
				obs->fire = fr;
				gp->addChild(fr, 900);
				fr->setPosition(ccpAdd(obs->getPosition(), ccp(tWidth/2,0)));
				//return;
			}
		}
		// guard
		CCMutableArray<Guard*>::CCMutableArrayIterator igd;
		Guard* gd;
		for (igd = gp->guards->begin(); igd != gp->guards->end(); igd++)
		{
			gd = *igd;
			if (!gd->inAction
				&& CCRect::CCRectIntersectsRect(gd->getRectOut(), getRectIn())
				&& CCRANDOM_0_1() < 0.3)
			{
				Fire* fr = Fire::fire();
				fr->owner = gd;
				gp->addChild(fr, 900);
				gd->setFire(fr);
			}
		}
		// thief
		CCMutableArray<Thief*>::CCMutableArrayIterator itf;
		Thief* tf;
		for (itf = gp->thieves->begin(); itf != gp->thieves->end(); itf++)
		{
			tf = *itf;
			if (!tf->inAction
				&& CCRect::CCRectIntersectsRect(tf->getRectOut(), getRectIn())
				&& CCRANDOM_0_1() < 0.3)
			{
				Fire* fr = Fire::fire();
				fr->owner = tf;
				gp->addChild(fr, 900);
				if (tf->gem)
				{
					gp->reorderChild(tf->gem, 0);
					tf->gem->setPosition(tf->getPosition());
					gp->gemsOutside->addObject(tf->gem);
					tf->gem = NULL;
				}
				tf->setFire(fr);
			}
		}
		// porter
		Porter* pt = gp->porter;
		if (pt && !pt->inAction
			&& CCRect::CCRectIntersectsRect(pt->getRectOut(), getRectIn())
			&& CCRANDOM_0_1() < 0.3)
		{
			Fire* fr = Fire::fire();
			fr->owner = pt;
			gp->addChild(fr, 900);
			if (pt->gem)
			{
				pt->gem->setPosition(pt->getPosition());
				gp->reorderChild(pt->gem, 0);
				gp->gemsOutside->addObject(pt->gem);
				pt->gem = NULL;
			}
			pt->setFire(fr);
		}
	}
}

CCRect Fire::getRectIn()
{
	//int w = sprite->getContentSize().width/2;
	//int h = PathFinder::getInstance()->tileHeight;
	return CCRectMake(getPosition().x - 10, getPosition().y + 6, 20, 20);	
}