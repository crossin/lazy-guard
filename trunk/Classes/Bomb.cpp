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
		timeLife = 5;
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