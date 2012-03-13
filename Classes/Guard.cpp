#include "Guard.h"
#include "PathFinder.h"
#include "Gameplay.h"
#include "cocos2d.h"

extern int game_map[10][15];

Guard::Guard(void)
{
}


Guard::~Guard(void)
{
}

Guard* Guard::guard()
{
	Guard *guard = new Guard;

	if (guard && guard->init())
	{
		guard->autorelease();
		return guard;
	}

	return NULL;
}

bool Guard::init()
{
	bool bRet = false;
	do{
		this->setAnchorPoint(CCPointZero);

		sprite = CCSprite::spriteWithFile("Player.png");
		sprite->setAnchorPoint(CCPointZero);
		this->addChild(sprite);

		bar = CCSprite::spriteWithFile("bar.png");
		bar->setAnchorPoint(CCPointZero);
		bar->setPosition(ccp(5,38));
		this->addChild(bar);
		//setPosition(ccp(160,160));

		//isAwake = false;
		status = SLEEPING;
		pointSleepMax = 100;
		pointSleep = 0;
		pointWakeMax = 100;
		pointWake = 0;
		speed = 140;
		range = 30;
		//behaviour=STAND;
		//direction=DOWN;

		//CCActionInterval *action=getAnimate(MOVE,OVERLOOK);
		//spirte->runAction(CCRepeatForever::actionWithAction(action));

		//this->schedule(schedule_selector(Monster::moveUpdate));
		schedule( schedule_selector(Guard::updateFrame));

		bRet=true;
	}while(0);
	return bRet;
}


void Guard::findThief() 
{
	CCMutableArray<Thief*>* thieves = ((Gameplay*)getParent())->thieves;
	CCMutableArray<Thief*>::CCMutableArrayIterator it;
	Thief* closest = NULL;
	Thief* thiefTemp;
	float dist;
	float dist_min = 100000;
	for (it = thieves->begin(); it != thieves->end(); it++ )
	{
		thiefTemp = *it;
		if (!thiefTemp->isFleeing && thiefTemp->inScreen)
		{
			dist = ccpDistance(getPosition(), thiefTemp->getPosition());
			if (dist < dist_min)
			{
				dist_min = dist;
				closest = thiefTemp;
			}
		}
	}

	if (!closest)
	{
		if (status != PATROLING)
		{
			patrol();
		}
		return;
	}

	PathFinder* pathfinder = PathFinder::getInstance();
	if (PathFinder::found != pathfinder->FindPath(getPosition().x,getPosition().y,closest->getPosition().x,closest->getPosition().y))
	{
		//isAwake = false;
		return;
	}

	//int tileWidth = pathfinder->tileWidth;
	//int tileHeight = pathfinder->tileHeight;
	//_itoa_s((*(pathBank[1]-4)),textout,10);
	//itoa(pathLength[1],textout,10);
	//CCArray* path = CCArray::array();
	CCPoint target, from;
	CCPoint moveDifference;
	float distanceToMove;
	float moveDuration;
	CCFiniteTimeAction* actionMove;

// 	if (pathfinder->pathLength<2)
// 	{
// 		return;
// 	}


// for (int i=0;i<20;i++){
// 	for (int j=0;j<30;j++){
// 			game_map[i][j] = 0;
// 	}
// }
// for (int i = 0; i < pathfinder->pathLength; i++)
// {
// game_map[pathfinder->pathBank[2*i+1]][pathfinder->pathBank[2*i]] = 2;
// }

	from = getPosition();//ccp(pathfinder->pathBank[0] * w, pathfinder->pathBank[1] * w);
	target = ccp(pathfinder->pathBank[0] * pathfinder->tileWidth, pathfinder->pathBank[1] * pathfinder->tileHeight);
	moveDifference = ccpSub(target,from);
	distanceToMove = ccpLength(moveDifference);
	moveDuration = distanceToMove/speed;
	actionMove = CCMoveTo::actionWithDuration((ccTime)moveDuration, target);

// char a[20];
// sprintf(a,"%5f,%d\n",moveDuration,pathfinder->pathLength);
// OutputDebugStringA(a);
// 
// if (moveDuration>1)
// {
// 	int t=1;
// }

	CCFiniteTimeAction* actionMoveDone = CCCallFuncN::actionWithTarget( this, callfuncN_selector(Guard::spriteMoveFinished));

	//CCFiniteTimeAction* actionMoveDone = CCCallFuncN::actionWithTarget( this, callfuncN_selector(HelloWorld::spriteMoveFinished));
	stopAllActions();
	runAction( CCSequence::actions(actionMove,actionMoveDone,NULL) );
	status = CHASING;
}

void Guard::patrol() 
{
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	int randX = CCRANDOM_0_1() * size.width;
	int randY = CCRANDOM_0_1() * size.height;
	PathFinder *pathfinder = PathFinder::getInstance();
	if (PathFinder::found != pathfinder->FindPath(getPosition().x, getPosition().y, randX, randY))
	{
		return;
	}

	CCArray* pathGo = CCArray::array();
	CCPoint target, from;
	CCPoint moveDifference;
	float distanceToMove;
	float moveDuration;
	CCFiniteTimeAction* actionMove;
	CCFiniteTimeAction* actionGo;

	for (int i = 0; i < pathfinder->pathLength; i++)
	{
		from = (i == 0) ? getPosition() : (ccp(pathfinder->pathBank[2*i-2] * pathfinder->tileWidth, pathfinder->pathBank[2*i-1] * pathfinder->tileHeight));
		target = ccp(pathfinder->pathBank[2*i] * pathfinder->tileWidth, pathfinder->pathBank[2*i+1] * pathfinder->tileHeight);
		moveDifference = ccpSub(target,from);
		distanceToMove = ccpLength(moveDifference);
		moveDuration = distanceToMove / speed;
		actionMove = CCMoveTo::actionWithDuration((ccTime)moveDuration, target);
		pathGo->addObject(actionMove);
	}

	actionGo = CCSequence::actionsWithArray(pathGo);
	CCFiniteTimeAction* actionWait = CCDelayTime::actionWithDuration(1);
//	CCFiniteTimeAction* actionOver = CCCallFuncN::actionWithTarget( this, callfuncN_selector(Thief::moveFinished));
	stopAllActions();
	runAction( CCSequence::actions(actionWait, actionGo, NULL) );
	status = PATROLING;
for (int i=0;i<10;i++){
 	for (int j=0;j<15;j++){
 			game_map[i][j] = 0;
 	}
 }
 for (int i = 0; i < pathfinder->pathLength; i++)
 {
 game_map[pathfinder->pathBank[2*i+1]][pathfinder->pathBank[2*i]] = 2;
 }
}

void Guard::updateFrame(ccTime dt)
{
	// update sleep/wake point
	if (status == SLEEPING)
	{
		if (pointSleep <= 0)
		{
			bar->setIsVisible(false);
		}
		else
		{
			bar->setTextureRect(CCRectMake(0, 0, 16*pointSleep/pointSleepMax, bar->getContentSize().height));
			bar->setIsVisible(true);
			pointSleep -= (50 * dt);
		}
	}
	else
	{
		if (pointWake <= 0)
		{
			stopAllActions();
			setAwake(false);
			bar->setIsVisible(false);
		}
		else
		{
			bar->setTextureRect(CCRectMake(0, 0, 16*pointWake/pointWakeMax, bar->getContentSize().height));
			bar->setIsVisible(true);
			pointWake -= (20 * dt);

			if (this->numberOfRunningActions() == 0)
			{
				status = WAKING;
			}
			if (status == WAKING || status == PATROLING)
			{
				findThief();
			}
		}
	}
	
}

void Guard::spriteMoveFinished(CCNode* sender)
{
	//CCSprite *sprite = (CCSprite *)sender;
	this->findThief();
}

CCRect Guard::getRect()
{
	return CCRectMake(getPosition().x, getPosition().y, sprite->getContentSize().width, sprite->getContentSize().height);
}

void Guard::onHit()
{
	pointSleep += 50;
	if (pointSleep >= pointSleepMax)
	{
		setAwake(true);
		findThief();
	}
}

void Guard::setAwake(bool w)
{
	if (w)
	{
		status = WAKING;
		//isAwake = true;
		pointWake = pointWakeMax;
	}
	else
	{
		status = SLEEPING;
		//isAwake = false;
		pointSleep = 0;
	}

}