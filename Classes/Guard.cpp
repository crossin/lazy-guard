#include "Guard.h"
#include "PathFinder.h"
/*#include "CCPointExtension.h"*/
#include "Gameplay.h"
// #include "CCGeometry.h"
// #include "selector_protocol.h"
#include "cocos2d.h"

		extern int game_map[20][30];

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

		setPosition(ccp(220,160));

		//behaviour=STAND;
		//direction=DOWN;

		//CCActionInterval *action=getAnimate(MOVE,OVERLOOK);
		//spirte->runAction(CCRepeatForever::actionWithAction(action));

		//this->schedule(schedule_selector(Monster::moveUpdate));
		bRet=true;
	}while(0);
	return bRet;
}


void Guard::findThief() 
{
	CCMutableArray<Thief*>* thieives = ((Gameplay*)getParent())->thieves;
	CCMutableArray<Thief*>::CCMutableArrayIterator it;
	Thief* closest = NULL;
	Thief* thiefTemp;
	float dist;
	float dist_min = 100000;
	for (it = thieives->begin(); it != thieives->end(); it++ )
	{
		thiefTemp = *it;
		dist = ccpDistance(getPosition(), thiefTemp->getPosition());
		if (dist < dist_min)
		{
			dist_min = dist;
			closest = thiefTemp;
		}
	}

	if (!closest)
	{
		return;
	}

	// check caught
	CCRect rectG = getRect();//CCRectMake(getPosition().x,getPosition().y,getContentSize().width,getContentSize().height);
	CCRect rectT = closest->getRect();//CCRectMake(closest->getPosition().x,closest->getPosition().y,closest->getContentSize().width,closest->getContentSize().height);

	if (CCRect::CCRectIntersectsRect(rectG,rectT))
	{
		if (closest->gem)
		{
			((Gameplay*)getParent())->gems->addObject(closest->gem);
			int i = ((Gameplay*)getParent())->gems->indexOfObject(closest->gem);
			closest->gem->setPosition(ccp(240+8*sin(i*6.28/5),160+8*cos(i*6.28/5)));
			closest->gem = NULL;
		}
		closest->kill();
		return;
	}

	PathFinder* pathfinder = PathFinder::getInstance();
	if ((pathfinder->FindPath(1,getPosition().x,getPosition().y,closest->getPosition().x,closest->getPosition().y)) == PathFinder::nonexistent)
	{
		return;
	}

	int w = 16;
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


for (int i=0;i<20;i++){
	for (int j=0;j<30;j++){
			game_map[i][j] = 0;
	}
}
for (int i = 0; i < pathfinder->pathLength; i++)
{
game_map[pathfinder->pathBank[2*i+1]][pathfinder->pathBank[2*i]] = 2;
}

	from = getPosition();//ccp(pathfinder->pathBank[0] * w, pathfinder->pathBank[1] * w);
	target = ccp(pathfinder->pathBank[0] * w, pathfinder->pathBank[1] * w);
	moveDifference = ccpSub(target,from);
	distanceToMove = ccpLength(moveDifference);
	moveDuration = distanceToMove/200;
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
	runAction( CCSequence::actions(actionMove,actionMoveDone,NULL) );
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