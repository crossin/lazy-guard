#include "Character.h"
#include "cocos2d.h"
// #include "Gameplay.h"

using namespace cocos2d;

const float Character::INTERVAL = 1;

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