#include "Bomb.h"
#include "CCActionInterval.h"
#include "AnimatePacker.h"
#include "Gameplay.h"
#include "PathFinder.h"


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
		timeLife = 0.5;
		scheduleUpdate();

		bRet=true;
	}while(0);

	return bRet;
}

void Bomb::kill()
{
	// add flame
	Gameplay* gp = (Gameplay*)getParent();
	int tWidth = PathFinder::getInstance()->tileWidth;
	int tHeight = PathFinder::getInstance()->tileHeight;
	int posX = (int)getPosition().x / tWidth;
	int posY = (int)getPosition().y / tHeight;
	for (int i = -2; i <= 2; i++)
	{
		for (int j = abs(i)-2; j <= 2-abs(i); j++)
		{

			Flame* fm = Flame::flame();
			fm->setPosition(ccp((posX+i+0.5)*tWidth, (posY+j+0.5)*tHeight));
			gp->addChild(fm, 901);
		}
	}

	// push guard
	CCMutableArray<Guard*>::CCMutableArrayIterator igd;
	Guard* gd;
	for (igd = gp->guards->begin(); igd != gp->guards->end(); igd++)
	{
		gd = *igd;
		if (abs(gd->getPosition().x-getPosition().x) + abs(gd->getPosition().y-getPosition().y) <= 2.5*tWidth){
			gd->setBomb(getPosition());
		}
	}
	// push thief
	CCMutableArray<Thief*>::CCMutableArrayIterator itf;
	Thief* tf;
	for (itf = gp->thieves->begin(); itf != gp->thieves->end(); itf++)
	{
		tf = *itf;
		if (abs(tf->getPosition().x-getPosition().x) + abs(tf->getPosition().y-getPosition().y) <= 2.5*tWidth)
		{
			if (tf->gem)
			{
				gp->reorderChild(tf->gem, 0);
				tf->gem->setPosition(tf->getPosition());
				gp->gemsOutside->addObject(tf->gem);
				tf->gem = NULL;
			}
			tf->setBomb(getPosition());
		}
	}
	// destroy obstacle
	CCMutableArray<Obstacle*>::CCMutableArrayIterator iobs;
	CCMutableArray<Obstacle*>* toDestroy = new CCMutableArray<Obstacle*>;
	Obstacle* obs;
	for (iobs = gp->obstacles->begin(); iobs != gp->obstacles->end(); iobs++)
	{
		obs = *iobs;
		if((obs->typeIndex==1 || obs->typeIndex==2)
			&& abs(obs->getPosition().x+tWidth/2-getPosition().x) + abs(obs->getPosition().y+tHeight/2-getPosition().y) <= 2.5*tWidth)
		{
			toDestroy->addObject(obs);
		}
	}
	for (iobs = toDestroy->begin(); iobs != toDestroy->end(); iobs++)
	{
		obs = *iobs;
		obs->kill();
	}
	toDestroy->release();

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

//////////////////////////////////////////////////////////////////////////
// Class Flame

Flame* Flame::flame()
{
	Flame* flame = (Flame*)CCSprite::spriteWithSpriteFrameName("flame1.png");
	CCAnimate* anim = AnimatePacker::getInstance()->getAnimate("flame");
	CCFiniteTimeAction* actionOver = CCCallFunc::actionWithTarget( flame, callfunc_selector(Flame::kill));
	flame->runAction(CCSequence::actions(anim, actionOver, NULL));
	return flame;
}

void Flame::kill()
{
	removeFromParentAndCleanup(true);
}