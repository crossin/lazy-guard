#include "Guard.h"
#include "PathFinder.h"
#include "Gameplay.h"
#include "cocos2d.h"
#include "AnimatePacker.h"

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
		//this->setAnchorPoint(CCPointZero);

		sprite=CCSprite::spriteWithSpriteFrameName("guard/guard-walk-down-0.png"); 
		sprite->setAnchorPoint(ccp(0.5,0)); 
		addChild(sprite);
		//sprite->setPosition(ccp(size.width/2, size.height/2)); 
		for (int i = 0; i < 8 ; i++)
		{
			char name[16];
			sprintf(name,"guard-walk-%d",i);
			actionWalks[i] = CCRepeatForever::actionWithAction(AnimatePacker::getInstance()->getAnimate(name));
			actionWalks[i]->retain();
		}
		actionWalk = actionWalks[4];
		//actionWalk->retain();
		//sprite->runAction(actionWalk); 


		bar = CCSprite::spriteWithFile("bar.png");
		bar->setAnchorPoint(CCPointZero);
		//
		//bar->setPosition(ccp(100,100));
		//this->addChild(bar);
		//setPosition(ccp(160,160));

		//isAwake = false;
		status = SLEEPING;
		pointSleepMax = 100;
		pointSleep = 0;
		pointWakeMax = 100;
		pointWake = 0;
		speed = 60;
		range = 30;
		//timeRot = 0.5;
		findingInterval = INTERVAL;
		//numClock = 0;
		onClock = false;
		inAction = false;
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
		if (thiefTemp->status != Thief::FLEEING && !thiefTemp->onFire && thiefTemp->inScreen())
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
	target = ccp((pathfinder->pathBank[0]+0.5) * pathfinder->tileWidth, (pathfinder->pathBank[1]+0.5) * pathfinder->tileHeight);
	actionMove = makeAction(from, target);

// char a[20];
// sprintf(a,"%5f,%d\n",moveDuration,pathfinder->pathLength);
// OutputDebugStringA(a);
// 
// if (moveDuration>1)
// {
// 	int t=1;
// }

	//CCFiniteTimeAction* actionMoveDone = CCCallFuncN::actionWithTarget( this, callfuncN_selector(Guard::spriteMoveFinished));

	stopAllActions();
	//runAction( CCSequence::actions(actionMove,/*actionMoveDone,*/NULL) );
	runAction(actionMove);
	status = CHASING;

// if (INTERVAL-findingInterval<0.5)
// {
// 	char textout[8];
// 	_itoa_s((INTERVAL-findingInterval)*100,textout,10);
// 	((Gameplay*)getParent())->pLabel->setString(textout);
// }

//	findingInterval = INTERVAL;


for (int i=0;i<10;i++){
	for (int j=0;j<15;j++){
		game_map[i][j] = 0;
	}
}
for (int i = 0; i < pathfinder->pathLength; i++){
game_map[pathfinder->pathBank[2*i+1]][pathfinder->pathBank[2*i]] = 2;
}

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
	runAction( CCSequence::actions(actionWait, actionGo, NULL) );
	status = PATROLING;
//	findingInterval = INTERVAL;

/*
char textout[10];
_itoa_s(pathfinder->pathLength,textout,10);
((Gameplay*)this->getParent())->pLabel->setString(textout);

 for (int i=0;i<10;i++){
  	for (int j=0;j<15;j++){
  			game_map[i][j] = 0;
  	}
  }
  for (int i = 0; i < pathfinder->pathLength; i++)
  {
  game_map[pathfinder->pathBank[2*i+1]][pathfinder->pathBank[2*i]] = 2;
  }
*/
}

void Guard::runWithFire() 
{
	if (numberOfRunningActions() > 0)
	{
		return;
	}
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
	CCFiniteTimeAction* actionGo;

	for (int i = 0; i < pathfinder->pathLength; i++)
	{
		from = (i == 0) ? getPosition() : (ccp((pathfinder->pathBank[2*i-2]+0.5) * pathfinder->tileWidth, (pathfinder->pathBank[2*i-1]+0.5) * pathfinder->tileHeight));
		target = ccp((pathfinder->pathBank[2*i]+0.5) * pathfinder->tileWidth, (pathfinder->pathBank[2*i+1]+0.5) * pathfinder->tileHeight);
		pathGo->addObject(makeAction(from, target));
	}

	actionGo = CCSequence::actionsWithArray(pathGo);
	//CCFiniteTimeAction* actionWait = CCDelayTime::actionWithDuration(1);
	//	CCFiniteTimeAction* actionOver = CCCallFuncN::actionWithTarget( this, callfuncN_selector(Thief::moveFinished));
	//stopAllActions();
	runAction( CCSequence::actions(actionGo, NULL) );
}

void Guard::updateFrame(ccTime dt)
{
	// update sleep/wake point
	if (status == SLEEPING)    // sleeping
	{
		if (pointSleep <= 0)
		{
			bar->setIsVisible(false);
		}
		else
		{
			bar->setTextureRect(CCRectMake(0, 0, 36*pointSleep/pointSleepMax, bar->getContentSize().height));
			bar->setIsVisible(true);
			pointSleep -= (50 * dt);
		}
	}
	else if (status == BURNING)    // burning
	{
		bar->setIsVisible(false);
		runWithFire();
	}
	else if (status != STUNNING)   // waiting chasing patroling
	{
		if (pointWake <= 0)
		{
			//stopAllActions();
			setAwake(false);
			bar->setIsVisible(false);
		}
		else
		{
			// awake
			findingInterval -= dt;
			// clock
			if (onClock)
			{
				bar->setIsVisible(false);
			}
			else
			{
				bar->setTextureRect(CCRectMake(0, 0, 36*pointWake/pointWakeMax, bar->getContentSize().height));
				bar->setIsVisible(true);
				pointWake -= (20 * dt);
			}

			if (numberOfRunningActions() == 0)
			{
				status = WAITING;
			}
			if (findingInterval < 0 || status == WAITING)
			{
				findThief();
				findingInterval = INTERVAL;
			}
		}
	}
	// update position of bar
	bar->setPosition(ccp(getPosition().x-18, getPosition().y+36));
}

// void Guard::spriteMoveFinished(CCNode* sender)
// {
// 	//CCSprite *sprite = (CCSprite *)sender;
// 	this->findThief();
// }


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
		status = WAITING;
		if (sprite->numberOfRunningActions()==0)
		{
			sprite->runAction(actionWalk);
		}
		//isAwake = true;
		pointWake = pointWakeMax;
	}
	else
	{
		status = SLEEPING;
		stopAllActions();
		sprite->stopAction(actionWalk);
		sprite->setDisplayFrameWithAnimationName("guard-walk-4", 0);
		//isAwake = false;
		pointSleep = 0;
	}
}

CCRect Guard::getRectClick()
{
	int border = 20;
	return CCRectMake(getPosition().x - sprite->getContentSize().width * sprite->getAnchorPoint().x - border,
		getPosition().y - sprite->getContentSize().height * sprite->getAnchorPoint().y - border,
		sprite->getContentSize().width + border * 2,
		sprite->getContentSize().height + border * 2);
}

void Guard::setClock( bool on )
{
	onClock = on;
	inAction = on;
	setAwake(onClock);
	//numClock = on ? numClock+1 : numClock-1;
	//setAwake(numClock > 0);
}

void Guard::setFire( bool on )
{
	inAction = on;
	setAwake(onClock);
	status = on ? BURNING : SLEEPING;
	stopAllActions();
}

void Guard::setBomb(CCPoint bPos)
{
	stopAllActions();
	bar->setIsVisible(false);
	sprite->stopAction(actionWalk);
	status = STUNNING;
	inAction = true;

	CCPoint direct = ccpSub(getPosition(), bPos);
	CCPoint newPos = getPosition();
	CCPoint dist = CCPointZero;
	CCPoint tempPos;
	int tWidth = PathFinder::getInstance()->tileWidth;
	int tHeight = PathFinder::getInstance()->tileHeight;
	for (int i=1; i<=4; i++)
	{
		dist = ccpMult(ccpNormalize(direct), i*tWidth/2);
		tempPos = ccpAdd(getPosition(), dist);
		if (PathFinder::getInstance()->isUnwalkable((int)tempPos.x/tWidth, (int)tempPos.y/tHeight))
		{
			break;
		}
		newPos = tempPos;
	}
	CCFiniteTimeAction* actionPush = CCMoveTo::actionWithDuration(ccpLength(dist)/300, newPos);
	runAction(actionPush);
	CCFiniteTimeAction* actionStun = CCDelayTime::actionWithDuration(2);
	CCFiniteTimeAction* actionWake = CCCallFunc::actionWithTarget( this, callfunc_selector(Guard::stunOver));
	runAction( CCSequence::actions(actionStun, actionWake, NULL) );


// 	inAction = on;
// 	setAwake(onClock);
// 	status = on ? BURNING : SLEEPING;
// 	stopAllActions();
}

void Guard::stunOver()
{
	inAction = false;
	setAwake(true);
}



