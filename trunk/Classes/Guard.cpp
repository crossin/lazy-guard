#include "Guard.h"
#include "PathFinder.h"
/*#include "CCPointExtension.h"*/
#include "Gameplay.h"
// #include "CCGeometry.h"
// #include "selector_protocol.h"
#include "cocos2d.h"


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

		setPosition(ccp(240,160));

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

	Thief* closest = ((Gameplay*)getParent())->thieves->getLastObject();

	// check caught
	CCRect rectG = CCRectMake(getPosition().x,getPosition().y,getContentSize().width,getContentSize().height);
	CCRect rectT = CCRectMake(closest->getPosition().x,closest->getPosition().y,closest->getContentSize().width,closest->getContentSize().height);

	if (CCRect::CCRectIntersectsRect(rectG,rectT))
	{
		return;
	}

	PathFinder* pathfinder = PathFinder::getInstance();
	pathfinder->FindPath(1,getPosition().x,getPosition().y,closest->getPosition().x,closest->getPosition().y);

	int w = 16;
	//_itoa_s((*(pathBank[1]-4)),textout,10);
	//itoa(pathLength[1],textout,10);
	CCArray* path = CCArray::array();
	CCPoint target, from;
	CCPoint moveDifference;
	float distanceToMove;
	float moveDuration;
	CCFiniteTimeAction* actionMove;

	if (pathfinder->pathLength<2)
	{
		return;
	}
	//	game_map[pathBank[1][3]][pathBank[1][2]] = 2;
	from = ccp(pathfinder->pathBank[0] * w, pathfinder->pathBank[1] * w);
	target = ccp(pathfinder->pathBank[2] * w, pathfinder->pathBank[3] * w);
	moveDifference = ccpSub(target,from);
	distanceToMove = ccpLength(moveDifference);
	moveDuration = distanceToMove/100;
	actionMove = CCMoveTo::actionWithDuration((ccTime)moveDuration, target);

	CCFiniteTimeAction* actionMoveDone = CCCallFuncN::actionWithTarget( this, callfuncN_selector(Guard::spriteMoveFinished));

	//CCFiniteTimeAction* actionMoveDone = CCCallFuncN::actionWithTarget( this, callfuncN_selector(HelloWorld::spriteMoveFinished));
	runAction( CCSequence::actions(actionMove,actionMoveDone,NULL) );
}

void Guard::spriteMoveFinished(CCNode* sender)
{
	CCSprite *sprite = (CCSprite *)sender;
	this->findThief();
}
