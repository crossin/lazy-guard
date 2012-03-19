#include "Character.h"
#include "cocos2d.h"

using namespace cocos2d;

Character::Character(void)
{
}

Character::~Character(void)
{
}

CCRect Character::getRect()
{
	return CCRectMake(getPosition().x - sprite->getContentSize().width/2,
		getPosition().y - sprite->getContentSize().height/2,
		sprite->getContentSize().width,
		sprite->getContentSize().height);
}

CCFiniteTimeAction* Character::makeAction( CCPoint from, CCPoint target )
{

	CCPoint moveDifference;
	float distanceToMove;
	float moveDuration;
	float rotAngle;
 	CCFiniteTimeAction* actionMove;
	CCFiniteTimeAction* actionRot;

	moveDifference = ccpSub(target,from);
	distanceToMove = ccpLength(moveDifference);
	moveDuration = distanceToMove / speed;
	actionMove = CCMoveTo::actionWithDuration((ccTime)moveDuration, target);
	//rotate
	rotAngle = CC_RADIANS_TO_DEGREES(ccpToAngle(moveDifference));
 	actionRot = CCRotateTo::actionWithDuration((timeRot<moveDuration?timeRot:moveDuration), 90-rotAngle);

	actionMove = CCSpawn::actions(actionMove, actionRot, NULL);

	return actionMove;
}
