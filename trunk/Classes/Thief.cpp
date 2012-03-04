#include "Thief.h"
#include "PathFinder.h"
#include "Gameplay.h"
#include "cocos2d.h"

Thief::Thief(void)
{
}

Thief::~Thief(void)
{
}

Thief* Thief::thief()
{
	Thief *thief = new Thief;

	if (thief && thief->init())
	{
		thief->autorelease();
		return thief;
	}

	return NULL;
}

bool Thief::init()
{
	bool bRet = false;
	do{
		this->setAnchorPoint(CCPointZero);

		sprite = CCSprite::spriteWithFile("Target.png");
		sprite->setAnchorPoint(CCPointZero);
		this->addChild(sprite);


		float ranPos = CCRANDOM_0_1();
		float startX, startY;
		//srand(GetTickCount());
		if (ranPos<0.5)
		{
			startX = CCRANDOM_0_1()*500;
			startY = (ranPos<0.25)?-20:340;
		} 
		else
		{
			startX = (ranPos<0.75)?-20:500;
			startY = CCRANDOM_0_1()*340;
		}

		setPosition(ccp(startX,startY));
		
		//behaviour=STAND;
		//direction=DOWN;

		//CCActionInterval *action=getAnimate(MOVE,OVERLOOK);
		//spirte->runAction(CCRepeatForever::actionWithAction(action));

		//this->schedule(schedule_selector(Monster::moveUpdate));
		bRet=true;
	}while(0);
	return bRet;
}

void Thief::findPath()
{
	PathFinder *pathfinder = PathFinder::getInstance();
	pathfinder->FindPath(1, getPosition().x, getPosition().y, 240, 160);
	int w = 16;
	//_itoa_s((*(pathBank[1]-4)),textout,10);
	//itoa(pathLength[1],textout,10);
	CCArray* pathGo = CCArray::array();
	CCArray* pathBack = CCArray::array();
	CCPoint target, from;
	CCPoint moveDifference;
	float distanceToMove;
	float moveDuration;
	CCFiniteTimeAction* actionMove;
	CCFiniteTimeAction* actionGo;
	CCFiniteTimeAction* actionBack;

	for (int i = 0; i < pathfinder->pathLength; i++)
	{
		//game_map[pathfinder->pathBank[2*i+1]][pathfinder->pathBank[2*i]] = 2;
		from = (i == 0) ? getPosition() : (ccp(pathfinder->pathBank[2*i-2] * w, pathfinder->pathBank[2*i-1] * w));
		target = ccp(pathfinder->pathBank[2*i] * w, pathfinder->pathBank[2*i+1] * w);
		moveDifference = ccpSub(target,from);
		distanceToMove = ccpLength(moveDifference);
		moveDuration = distanceToMove/20;
		actionMove = CCMoveTo::actionWithDuration((ccTime)moveDuration, target);
		pathGo->addObject(actionMove);
		actionMove = CCMoveTo::actionWithDuration((ccTime)moveDuration, from);
		pathBack->addObject(actionMove);
	}
	//pLabel->setString(textout);
	//player->setPosition(m_tTouchPos);
	pathBack->reverseObjects();
	actionGo = CCSequence::actionsWithArray(pathGo);
	actionBack = CCSequence::actionsWithArray(pathBack);
	//CCFiniteTimeAction* actionMoveDone = CCCallFuncN::actionWithTarget( this, callfuncN_selector(HelloWorld::spriteMoveFinished));
	CCFiniteTimeAction* actionOver = CCCallFuncN::actionWithTarget( this, callfuncN_selector(Thief::moveFinished));
	
	runAction( CCSequence::actions(actionGo, actionBack, actionOver, NULL) );
}

void Thief::moveFinished(CCNode* sender)
{
	kill();
}

void Thief::kill()
{
	((Gameplay*)getParent())->thieves->removeObject(this);
	removeFromParentAndCleanup(true);
}

CCRect Thief::getRect()
{
	return CCRectMake(getPosition().x, getPosition().y, sprite->getContentSize().width, sprite->getContentSize().height);
}