#include "Thief.h"
#include "PathFinder.h"
#include "Gameplay.h"
#include "cocos2d.h"
#include "AnimatePacker.h"


extern int game_map[10][15];

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
		//this->setAnchorPoint(CCPointZero);
		AnimatePacker::getInstance()->loadAnimate("sprites.xml"); 
		sprite=CCSprite::spriteWithSpriteFrameName("thief2.png"); 
		//sprite->setAnchorPoint(CCPointZero); 
		//sprite->setPosition(ccp(size.width/2, size.height/2)); 
		sprite->runAction(CCRepeatForever::actionWithAction(AnimatePacker::getInstance()->getAnimate("thief-walk"))); 
		addChild(sprite); 

// 		sprite = CCSprite::spriteWithFile("Target.png");
// 		this->addChild(sprite);


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
// 		isFleeing = false;
		speed = 50;
		timeRot = 0.3;
		status = FINDING;
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
	CCArray* gemsOut = ((Gameplay*)getParent())->gemsOutside;
	Treasure* trs = ((Gameplay*)getParent())->treasure;
	int gemX = 0;
	int gemY = 0;
	float dist;
	float dist_min = 100000;
	if (gemsOut->count() > 0)
	{
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
		if (trs->gems->count() > 0 && status != BACKING)
		{
			dist = ccpDistance(getPosition(), ccp(trs->posX,trs->posY));
			if (dist < dist_min)
			{
				dist_min = dist;
				gemX = trs->posX;
				gemY = trs->posY;
			}
		} 
	} 
	else
	{
		if (status == BACKING)
		{
			return;
		}
		gemX = trs->posX;
		gemY = trs->posY;
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

void Thief::getGem(CCNode* sender)
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
	if (gem == NULL && CCRect::CCRectContainsPoint(this->getRect(), ccp(trs->posX, trs->posY)))
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

void Thief::findHome()
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


void Thief::fleeHome()
{
// 	isFleeing = true;
	status = FLEEING;
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