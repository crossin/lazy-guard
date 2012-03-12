#include "Thief.h"
#include "PathFinder.h"
#include "Gameplay.h"
#include "cocos2d.h"

//extern int game_map[10][15];

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
		//float startX, startY;
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
		gem = NULL;
		isFleeing = false;
		speed = 50;
		//behaviour=STAND;
		//direction=DOWN;

		schedule( schedule_selector(Thief::updateFrame));

		//CCActionInterval *action=getAnimate(MOVE,OVERLOOK);
		//spirte->runAction(CCRepeatForever::actionWithAction(action));

		//this->schedule(schedule_selector(Monster::moveUpdate));
		bRet=true;
	}while(0);
	return bRet;
}

void Thief::findGem()
{
	PathFinder *pathfinder = PathFinder::getInstance();
	Treasure* treasure = ((Gameplay*)getParent())->treasure;
	if (PathFinder::nonexistent == pathfinder->FindPath(getPosition().x, getPosition().y, treasure->posX, treasure->posY))
	{
		return;
	}
	//int w = 32;
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
		from = (i == 0) ? getPosition() : (ccp(pathfinder->pathBank[2*i-2] * pathfinder->tileWidth, pathfinder->pathBank[2*i-1] * pathfinder->tileHeight));
		target = ccp(pathfinder->pathBank[2*i] * pathfinder->tileWidth, pathfinder->pathBank[2*i+1] * pathfinder->tileHeight);
		moveDifference = ccpSub(target,from);
		distanceToMove = ccpLength(moveDifference);
		moveDuration = distanceToMove / speed;
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
	//CCFiniteTimeAction* actionOver = CCCallFuncN::actionWithTarget( this, callfuncN_selector(Thief::moveFinished));
	CCFiniteTimeAction* steal = CCCallFuncN::actionWithTarget( this, callfuncN_selector(Thief::getGem));

	stopAllActions();
	runAction( CCSequence::actions(actionGo, steal,/* actionBack, actionOver,*/ NULL) );

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

void Thief::getGem(CCNode* sender)
{
	CCArray* gems = ((Gameplay*)getParent())->treasure->gems;
	if (gem = (Gem*)gems->lastObject())
	{
		((Gameplay*)getParent())->reorderChild(gem, 1000);
		gems->removeLastObject();
	}
	findHome();
}

void Thief::findHome()
{
	PathFinder *pathfinder = PathFinder::getInstance();
	if (PathFinder::nonexistent == pathfinder->FindPath(getPosition().x, getPosition().y, startX, startY))
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
		target = (i == pathfinder->pathLength-1) ? ccp(startX, startY) : (ccp(pathfinder->pathBank[2*i] * pathfinder->tileWidth, pathfinder->pathBank[2*i+1] * pathfinder->tileHeight));
		moveDifference = ccpSub(target,from);
		distanceToMove = ccpLength(moveDifference);
		moveDuration = distanceToMove / speed;
		actionMove = CCMoveTo::actionWithDuration((ccTime)moveDuration, target);
		pathGo->addObject(actionMove);
	}

	actionGo = CCSequence::actionsWithArray(pathGo);
	CCFiniteTimeAction* actionOver = CCCallFuncN::actionWithTarget( this, callfuncN_selector(Thief::moveFinished));

	stopAllActions();
	runAction( CCSequence::actions(actionGo, actionOver, NULL) );
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

void Thief::moveFinished(CCNode* sender)
{
	kill();
}

void Thief::kill()
{
	if (gem)
	{
		gem->kill();
		((Gameplay*)getParent())->countGem--;
	}
	((Gameplay*)getParent())->thieves->removeObject(this);
	removeFromParentAndCleanup(true);
}

CCRect Thief::getRect()
{
	return CCRectMake(getPosition().x, getPosition().y, sprite->getContentSize().width, sprite->getContentSize().height);
}

void Thief::fleeHome()
{
	isFleeing = true;
	speed *= 2;
	//this->stopAllActions();
	this->findHome();
}

void Thief::updateFrame(ccTime dt)
{
	if (gem)
	{
		gem->setPosition(getPosition());
	}
}

bool Thief::inScreen()
{
	bool bRet;CCDirector::sharedDirector()->getWinSize().
	bRet = (getPosition().x<0 || getPosition().x+sprite->getContentSize().x>)
}