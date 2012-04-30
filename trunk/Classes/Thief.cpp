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

Thief* Thief::thief(int ts)
{
	Thief *thief = new Thief;

	if (thief && thief->init(ts))
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


		sprite=CCSprite::spriteWithSpriteFrameName("thief/thief-walk-down-0.png"); 
		sprite->setAnchorPoint(ccp(0.5,0)); 
		addChild(sprite);
		//sprite->setPosition(ccp(size.width/2, size.height/2)); 
		for (int i = 0; i < 8 ; i++)
		{
			char name[16];
			sprintf(name,"thief-walk-%d",i);
			actionWalks[i] = CCRepeatForever::actionWithAction(AnimatePacker::getInstance()->getAnimate(name));
			actionWalks[i]->retain();
		}
		actionWalk = actionWalks[4];

// 		sprite = CCSprite::spriteWithFile("Target.png");
// 		this->addChild(sprite);


// 		float ranPos = CCRANDOM_0_1();
// 		if (ranPos<0.5)
// 		{
// 			startX = CCRANDOM_0_1()*500;
// 			startY = (ranPos<0.25)?-20:340;
// 		} 
// 		else
// 		{
// 			startX = (ranPos<0.75)?-20:500;
// 			startY = CCRANDOM_0_1()*340;
// 		}
// 		setPosition(ccp(startX,startY));

		gem = NULL;
		clock = NULL;
		actionSpeed = NULL;
		speed = 50;
		speedFactor = 1;
		status = FINDING;
		findingInterval = INTERVAL;
		hasVisited = false;

		schedule( schedule_selector(Thief::updateFrame));

		bRet=true;
	}while(0);
	return bRet;
}

bool Thief::init( int ts )
{
	timeStart = ts;
	return init();
}

void Thief::findGem()
{
	CCArray* gemsOut = ((Gameplay*)getParent())->gemsOutside;
	Treasure* trs = ((Gameplay*)getParent())->treasure;
	Porter* pt = ((Gameplay*)getParent())->porter;
	int gemX = 0;
	int gemY = 0;
	float dist;
	float dist_min = 100000;
	if (gemsOut->count() > 0 || pt->gem)
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
		if (pt->gem)
		{
			dist = ccpDistance(getPosition(), pt->getPosition());
			if (dist < dist_min)
			{
				dist_min = dist;
				gemX = pt->getPosition().x;
				gemY = pt->getPosition().y;
			}
		}
		if (trs->gems->count() > 0/* && !hasVisited*/)
		{
			dist = ccpDistance(getPosition(), trs->getPosition());
			if (dist < dist_min)
			{
				dist_min = dist;
				gemX = trs->getPosition().x;
				gemY = trs->getPosition().y;
			}
		} 
	} 
	else
	{
		if (trs->gems->count() > 0 || !hasVisited)
		{
			gemX = trs->getPosition().x;
			gemY = trs->getPosition().y;
		}
		else
		{
			if (status != BACKING)
			{
				findHome();
			}
			return;
		}
	}

	PathFinder *pathfinder = PathFinder::getInstance();
	int result = pathfinder->FindPath(getPosition().x, getPosition().y, gemX, gemY);
	if (result == PathFinder::nonexistent)
	{
		return;
	}
	//int w = 32;
	//_itoa_s((*(pathBank[1]-4)),textout,10);
	//itoa(pathLength[1],textout,10);
	CCArray* pathGo = CCArray::array();
	CCPoint target, from;
	CCFiniteTimeAction* actionGo;
	if (result == PathFinder::same)
	{
		from = getPosition();
		target = ccp(gemX, gemY);
		pathGo->addObject(makeAction(from, target));
	} 
	else
	{
		for (int i = 0; i < pathfinder->pathLength; i++)
		{
			from = (i == 0) ? getPosition() : (ccp((pathfinder->pathBank[2*i-2]+0.5) * pathfinder->tileWidth, (pathfinder->pathBank[2*i-1]+0.5) * pathfinder->tileHeight));
			target = (i == pathfinder->pathLength-1) ? ccp(gemX, gemY) : (ccp((pathfinder->pathBank[2*i]+0.5) * pathfinder->tileWidth, (pathfinder->pathBank[2*i+1]+0.5) * pathfinder->tileHeight));
			pathGo->addObject(makeAction(from, target));
		}
	}
	actionGo = CCSequence::actionsWithArray(pathGo);
	//CCFiniteTimeAction* steal = CCCallFuncN::actionWithTarget( this, callfuncN_selector(Thief::getGem));
	actionSpeed = CCSpeed::actionWithAction((CCActionInterval*)actionGo, speedFactor);
	stopAllActions();
	//runAction( CCSequence::actions(actionGo, steal, /*actionBack, actionOver,*/ NULL) );
	runAction(actionSpeed);
	status = FINDING;
//	findingInterval = INTERVAL;
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

/*
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
*/

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
	CCFiniteTimeAction* actionOver = CCCallFunc::actionWithTarget( this, callfunc_selector(Thief::moveFinished));
	actionSpeed = CCSpeed::actionWithAction((CCActionInterval*)CCSequence::actions(actionGo, actionOver, NULL), speedFactor);
	stopAllActions();
	runAction( actionSpeed );
	status = BACKING;
//	findingInterval = INTERVAL;
// 	((Gameplay*)getParent())->updateThieves();
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

void Thief::moveFinished()
{
	kill();
}

void Thief::kill()
{
	Gameplay* gp = (Gameplay*)getParent();
	if (gem)
	{
		gem->kill();
		gp->countGem--;
	}
	if (clock)
	{
		clock->kill();
	}
	gp->thieves->removeObject(this);
	gp->things->removeObject(this);
	removeFromParentAndCleanup(true);
}


void Thief::fleeHome()
{
// 	isFleeing = true;
	speed *= 2;
	//this->stopAllActions();
	findHome();
	status = FLEEING;
}

void Thief::updateFrame(ccTime dt)
{
	if (gem)
	{
		gem->setPosition(ccp(getPosition().x, getPosition().y+16));
	}
	else if (status == FINDING || status == BACKING)
	{
		findingInterval -= dt;
		if (findingInterval < 0)
		{
			findGem();
			findingInterval = INTERVAL;
		}
	}
// 	if (numberOfRunningActions() == 0 && status == BACKING)
// 	{
// 		findHome();
// 	}
}

// void Thief::checkStart(ccTime dt)
// {
// 	timeLife += dt;
// 	if (timeLife >= timeStart)
// 	{
// // 		Thief *thief = Thief::thief();
// // 		thieves->addObject(thief);
// // 		
// // 		things->addObject(thief);
// // 		thief->findGem();
// // 		unschedule(schedule_selector(Thief::checkStart));
// // 		schedule(schedule_selector(Thief::updateFrame));	
// 	}
// }



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

void Thief::setClock( bool on )
{
	speedFactor = on ? 2 : 1;
	if (actionSpeed)
	{
		actionSpeed->setSpeed(speedFactor);
	}
	if (!on)
	{
		clock = NULL;
	}
}
