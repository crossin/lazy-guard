#pragma once

#include "CCSprite.h"

using namespace cocos2d;

class Thief :
	public CCNode
{
public:
	Thief(void);
	~Thief(void);

	static Thief* thief();
	bool init();

	void findPath();
	void moveFinished(CCNode* sender);
	void kill();
	CCRect getRect();

	CCSprite * sprite;
};
