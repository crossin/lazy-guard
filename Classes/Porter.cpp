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
		sprite=CCSprite::spriteWithSpriteFrameName("porter/porter-walk-down-0.png"); 
		sprite->setAnchorPoint(ccp(0.5,0.25)); 
		addChild(sprite);

		for (int i = 0; i < 8 ; i++)
		{
			char name[16];
			sprintf(name,"porter-walk-%d",i);
			actionWalks[i] = CCRepeatForever::actionWithAction(AnimatePacker::getInstance()->getAnimate(name));
			actionWalks[i]->retain();
		}
		actionWalk = actionWalks[4];

		setPosition(ccp(240, 120));
		gem = NULL;
		speed = 30;
		status = WAITING;
		findingInterval = INTERVAL;

		schedule( schedule_selector(Porter::updateFrame));

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
		if (status != PATROLING)
		{
			patrol();
		}
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
	//CCFiniteTimeAction* steal = CCCallFuncN::actionWithTarget( this, callfuncN_selector(Thief::getGem));

	stopAllActions();
	runAction(actionGo);
	status = FINDING;
	findingInterval = INTERVAL;

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

void Porter::findHome()
{
	PathFinder *pathfinder = PathFinder::getInstance();
	CCPoint pos = ((Gameplay*)getParent())->treasure->getPosition();
	int result = pathfinder->FindPath(getPosition().x, getPosition().y, pos.x, pos.y);
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
		target = ccp(pos.x, pos.y);
		pathGo->addObject(makeAction(from, target));
	}
	else
	{
		for (int i = 0; i < pathfinder->pathLength; i++)
		{
			from = (i == 0) ? getPosition() : (ccp((pathfinder->pathBank[2*i-2]+0.5) * pathfinder->tileWidth, (pathfinder->pathBank[2*i-1]+0.5) * pathfinder->tileHeight));
			target = (i == pathfinder->pathLength-1) ? ccp(pos.x, pos.y) : (ccp((pathfinder->pathBank[2*i]+0.5) * pathfinder->tileWidth, (pathfinder->pathBank[2*i+1]+0.5) * pathfinder->tileHeight));
			pathGo->addObject(makeAction(from, target));
		}
	}

	actionGo = CCSequence::actionsWithArray(pathGo);
	//CCFiniteTimeAction* actionOver = CCCallFunc::actionWithTarget( this, callfunc_selector(Porter::returnGem));

	stopAllActions();
	runAction(actionGo);
	status = BACKING;
	//runAction( CCSequence::actions(actionGo, actionOver, NULL) );


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


void Porter::patrol() 
{
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	int randX = (CCRANDOM_0_1()/2+0.25) * size.width;
	int randY = (CCRANDOM_0_1()/2+0.25) * size.height;
	PathFinder *pathfinder = PathFinder::getInstance();
	if (PathFinder::found != pathfinder->FindPath(getPosition().x, getPosition().y, randX, randY))
	{
		return;
	}

	CCArray* pathGo = CCArray::array();
	CCPoint target, from;
	CCFiniteTimeAction* actionGo;

	for (int i = 0; i < pathfinder->pathLength; i++)
	{
		from = (i == 0) ? getPosition() : (ccp((pathfinder->pathBank[2*i-2]+0.5) * pathfinder->tileWidth, (pathfinder->pathBank[2*i-1]+0.5) * pathfinder->tileHeight));
		target = ccp((pathfinder->pathBank[2*i]+0.5) * pathfinder->tileWidth, (pathfinder->pathBank[2*i+1]+0.5) * pathfinder->tileHeight);
		pathGo->addObject(makeAction(from, target));
	}

	actionGo = CCSequence::actionsWithArray(pathGo);
	CCFiniteTimeAction* actionWait = CCDelayTime::actionWithDuration(1);
	//	CCFiniteTimeAction* actionOver = CCCallFuncN::actionWithTarget( this, callfuncN_selector(Thief::moveFinished));
	stopAllActions();
	runAction( CCSequence::actions(actionGo, actionWait, NULL) );
	status = PATROLING;
	findingInterval = INTERVAL;
}

void Porter::updateFrame(ccTime dt)
{
	if (gem)
	{
		gem->setPosition(getPosition());
	}
	if (status == FINDING || status == PATROLING)
	{
		findingInterval -= dt;

		if (findingInterval < 0)
		{
			findGem();
		}
	}
	if (numberOfRunningActions() == 0)
	{
		status = WAITING;
	}
	if (status == WAITING)
	{
		findGem();
	}

}

