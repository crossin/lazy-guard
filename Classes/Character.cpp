#include "Character.h"
#include "cocos2d.h"
// #include "Gameplay.h"
#include "PathFinder.h"

using namespace cocos2d;

const float Character::INTERVAL = 0.7;

Character::Character(void)
{
}

Character::~Character(void)
{
	for (int i = 0; i < 8; i++)
	{
		actionWalks[i]->release();
	}
}

CCFiniteTimeAction* Character::makeAction( CCPoint from, CCPoint target )
{

	CCPoint moveDifference;
	float distanceToMove;
	float moveDuration;
	float moveAngle;
	int direction;
 	CCFiniteTimeAction* actionMove;
	CCFiniteTimeAction* actionRot;

	moveDifference = ccpSub(target,from);
	distanceToMove = ccpLength(moveDifference);
	moveDuration = distanceToMove / speed;
	actionMove = CCMoveTo::actionWithDuration((ccTime)moveDuration, target);
	//rotate
	moveAngle = 90-CC_RADIANS_TO_DEGREES(ccpToAngle(moveDifference))+23+360;
	direction = moveAngle / 45;
	direction %= 8;
 //	actionRot = CCRotateTo::actionWithDuration((timeRot<moveDuration?timeRot:moveDuration), 90-moveAngle);
	actionRot = CCCallFuncND::actionWithTarget( this, callfuncND_selector(Character::changeDirection), (void*)direction);

	actionMove = CCSpawn::actions(actionMove, actionRot, NULL);


	return actionMove;
}

void Character::changeDirection(CCNode *node, void *param)
{
	sprite->stopAction(actionWalk);
	int i = (int)param;
	actionWalk = actionWalks[i];
	sprite->runAction(actionWalk);
}

CCRect Character::getRectIn()
{
	int w = sprite->getContentSize().width;
	//int h = PathFinder::getInstance()->tileHeight;
	return CCRectMake(getPosition().x - w/2, getPosition().y - w/4,	w, w/2);	
}

void Character::runWithFire() 
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
	runAction( actionGo );
}

void Character::setClock( Clock* clk )
{
	// to be override
}

void Character::setFire( Fire* fr )
{
	// to be override
}

void Character::setBomb(CCPoint bPos)
{
	if (fire)
	{
		fire->kill();
	}
	if (clock)
	{
		clock->kill();
	}

	stopAllActions();
	sprite->stopAction(actionWalk);
	onBomb = true;
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
	CCFiniteTimeAction* actionWake = CCCallFunc::actionWithTarget( this, callfunc_selector(Character::stunOver));
	runAction( CCSequence::actions(actionStun, actionWake, NULL) );
}