#include "Porter.h"
#include "PathFinder.h"
#include "Gameplay.h"
#include "cocos2d.h"
#include "AnimatePacker.h"


extern int game_map[10][15];

Porter::Porter(void)
{
}

Porter::~Porter(void)
{
}

Porter* Porter::porter()
{
	Porter *porter = new Porter;

	if (porter && porter->init())
	{
		porter->autorelease();
		return porter;
	}

	return NULL;
}

bool Porter::init()
{
	bool bRet = false;
	do{
		sprite=CCSprite::spriteWithSpriteFrameName("thief/thief-walk-down-0.png"); 
		sprite->setAnchorPoint(ccp(0.5,0.25)); 
		addChild(sprite);

		for (int i = 0; i < 8 ; i++)
		{
			char name[13];
			sprintf(name,"thief-walk-%d",i);
			actionWalks[i] = CCRepeatForever::actionWithAction(AnimatePacker::getInstance()->getAnimate(name));
			actionWalks[i]->retain();
		}
		actionWalk = actionWalks[4];

		setPosition(ccp(240,320));
		gem = NULL;
		speed = 50;
		status = PATROLING;

		schedule( schedule_selector(Thief::updateFrame));

		bRet=true;
	}while(0);
	return bRet;
}

void Porter::findGem()
{
	CCArray* gemsOut = ((Gameplay*)getParent())->gemsOutside;
	int gemX = 0;
	int gemY = 0;
	float dist;
	float dist_min = 100000;
	if (gemsOut->count() == 0)
	{
		return;
	}

	Gem* g;
	for (int i=0; i<gemsOut->count(); i++)
	{
		g = (Gem*)gemsOut->objectAtIndex(i);
		dist = ccpDistance(getPosition(), g->getPosition());
		if (dist < dist_min)
		{
			dist_min = dist;
			gemX = g->getPosition().x;
			gemY = g->getPosition().y;
		}
	}

	PathFinder *pathfinder = PathFinder::getInstance();
	
	if (PathFinder::found != pathfinder->FindPath(getPosition().x, getPosition().y, gemX, gemY))
	{
		return;
	}
	//int w = 32;
	//_itoa_s((*(pathBank[1]-4)),textout,10);
	//itoa(pathLength[1],textout,10);
	CCArray* pathGo = CCArray::array();
	CCPoint target, from;
	CCFiniteTimeAction* actionGo;

	for (int i = 0; i < pathfinder->pathLength; i++)
	{
		//game_map[pathfinder->pathBank[2*i+1]][pathfinder->pathBank[2*i]] = 2;
		from = (i == 0) ? getPosition() : (ccp((pathfinder->pathBank[2*i-2]+0.5) * pathfinder->tileWidth, (pathfinder->pathBank[2*i-1]+0.5) * pathfinder->tileHeight));
		target = (i == pathfinder->pathLength-1) ? ccp(gemX, gemY) : (ccp((pathfinder->pathBank[2*i]+0.5) * pathfinder->tileWidth, (pathfinder->pathBank[2*i+1]+0.5) * pathfinder->tileHeight));
		pathGo->addObject(makeAction(from, target));
	}
	actionGo = CCSequence::actionsWithArray(pathGo);
	CCFiniteTimeAction* steal = CCCallFuncN::actionWithTarget( this, callfuncN_selector(Thief::getGem));

	stopAllActions();
	runAction( CCSequence::actions(actionGo, steal, /*actionBack, actionOver,*/ NULL) );

/*
 for (int i=0;i<10;i++){
 	for (int j=0;j<15;j++){
 		if (game_map[i][j] == 2)
 		{
 			game_map[i][j] = 0;
 		}
 	}
 }
 for (int i = 0; i < pathfinder->pathLength; i++)
 {
 	game_map[pathfinder->pathBank[2*i+1]][pathfinder->pathBank[2*i]] = 2;
 }
*/

}

void Porter::getGem(CCNode* sender)
{
// 	CCRect rectGem;
// 	CCRect rectThief;
	Gem* g;
	CCArray* gemsList = ((Gameplay*)getParent())->gemsOutside;
// 	rectThief = CCRectMake(this->getPosition().x, this->getPosition().y, this->sprite->getContentSize().width, this->sprite->getContentSize().height);
	// outside
	for (int i=0; i<gemsList->count(); i++)
	{
		g = (Gem*)gemsList->objectAtIndex(i);
// 		rectGem = CCRectMake(g->getPosition().x, g->getPosition().y, g->sprite->getContentSize().width, g->sprite->getContentSize().height);
		if (CCRect::CCRectIntersectsRect(g->getRect(), this->getRect()))
		{
			((Gameplay*)getParent())->reorderChild(g, 1000);
						gemsList->removeObject(g);
			gem = g;
			status = STEALING;
			this->findHome();
			break;
		}
	}
	// in treasure
	Treasure* trs = ((Gameplay*)getParent())->treasure;
	if (gem == NULL && CCRect::CCRectContainsPoint(this->getRect(), trs->getPosition()))
	{
		status = BACKING;
		gemsList = trs->gems;
		if (gem = (Gem*)gemsList->lastObject())
		{
			((Gameplay*)getParent())->reorderChild(gem, 1000);
			gemsList->removeLastObject();
			status = STEALING;
		}
		this->findHome();
	}
}

void Porter::findHome()
{
	PathFinder *pathfinder = PathFinder::getInstance();
	int result = pathfinder->FindPath(getPosition().x, getPosition().y, startX, startY);
	if (result == PathFinder::nonexistent)
	{
		return;
	}
	CCArray* pathGo = CCArray::array();
	CCPoint target, from;
	CCFiniteTimeAction* actionGo;
	if (result == PathFinder::same)
	{
		from = getPosition();
		target = ccp(startX, startY);
		pathGo->addObject(makeAction(from, target));
	}
	else
	{
		for (int i = 0; i < pathfinder->pathLength; i++)
		{
			from = (i == 0) ? getPosition() : (ccp((pathfinder->pathBank[2*i-2]+0.5) * pathfinder->tileWidth, (pathfinder->pathBank[2*i-1]+0.5) * pathfinder->tileHeight));
			target = (i == pathfinder->pathLength-1) ? ccp(startX, startY) : (ccp((pathfinder->pathBank[2*i]+0.5) * pathfinder->tileWidth, (pathfinder->pathBank[2*i+1]+0.5) * pathfinder->tileHeight));
			pathGo->addObject(makeAction(from, target));
		}
	}

	actionGo = CCSequence::actionsWithArray(pathGo);
	CCFiniteTimeAction* actionOver = CCCallFuncN::actionWithTarget( this, callfuncN_selector(Thief::moveFinished));

	stopAllActions();
	runAction( CCSequence::actions(actionGo, actionOver, NULL) );

	((Gameplay*)getParent())->updateThieves();
/*
for (int i=0;i<10;i++){
	for (int j=0;j<15;j++){
		if (game_map[i][j] == 2)
		{
			game_map[i][j] = 0;
		}
	}
}
for (int i = 0; i < pathfinder->pathLength; i++)
{
	game_map[pathfinder->pathBank[2*i+1]][pathfinder->pathBank[2*i]] = 2;
}
*/
}

// void Porter::moveFinished(CCNode* sender)
// {
// 	kill();
// }

// void Porter::kill()
// {
// 	Gameplay* gp = (Gameplay*)getParent();
// 	if (gem)
// 	{
// 		gem->kill();
// 		gp->countGem--;
// 	}
// 	gp->thieves->removeObject(this);
// 	gp->things->removeObject(this);
// 	removeFromParentAndCleanup(true);
// }


void Porter::fleeHome()
{
// 	isFleeing = true;
	status = FLEEING;
	speed *= 2;
	//this->stopAllActions();
	this->findHome();
}

void Porter::updateFrame(ccTime dt)
{
	if (gem)
	{
		gem->setPosition(getPosition());
	}
}

bool Porter::inScreen()
{
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
// 	int w = PathFinder::getInstance()->tileWidth / 2;
// 	int h = PathFinder::getInstance()->tileHeight / 2;
	return getPosition().x > 0 &&
		getPosition().x < winSize.width &&
		getPosition().y > 0 &&
		getPosition().y < winSize.height;
}

// void Thief::updateTarget()
// {
// 	if (status == FINDING || status == BACKING)
// 	{
// 		findGem();
// 	}
// }
