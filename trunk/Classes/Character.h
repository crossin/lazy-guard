#pragma once

#include "CCSprite.h"
#include "CCAction.h"

using namespace cocos2d;

class Character :
	public CCNode
{
public:
	Character(void);
	~Character(void);

	virtual bool init() = 0;

	CCRect getRect();
	CCFiniteTimeAction* makeAction(CCPoint from, CCPoint target);

	CCSprite* sprite;
	int speed;
	float timeRot;
};