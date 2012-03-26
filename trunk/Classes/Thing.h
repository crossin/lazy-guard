#pragma once

#include "CCSprite.h"

using namespace cocos2d;

class Thing :
	public CCNode
{
public:
	Thing(void);
	~Thing(void);

	static const int GUARD = 1, THIEF = 2;//, GEM = 3; 

	virtual bool init() = 0;

	CCRect getRect();

	CCSprite* sprite;
};