#pragma once

#include "Thing.h"
#include "CCAction.h"

using namespace cocos2d;

class Character :
	public Thing
{
public:
	Character(void);
	~Character(void);

	static const float INTERVAL;

	virtual bool init() = 0;

	CCFiniteTimeAction* makeAction(CCPoint from, CCPoint target);
	void changeDirection(CCNode *node, void *param);
	CCRect getRectIn();
	int speed;
	CCFiniteTimeAction* actionWalks[8];
	CCFiniteTimeAction* actionWalk;
};
