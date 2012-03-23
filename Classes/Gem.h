#pragma once

#include "CCSprite.h"

using namespace cocos2d;

class Gem :
	public CCNode
{
public:
	Gem(void);
	~Gem(void);

	static Gem* gem();

	bool init();
	void kill();
	CCRect getRect();

	CCSprite* sprite;
	//Thief* owner;
};

