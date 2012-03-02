#pragma once

#include "CCSprite.h"

using namespace cocos2d;

class Guard :
	public CCNode
{
public:
	Guard(void);
	~Guard(void);

	static Guard* guard();
	bool init();

	void findThief();
	void spriteMoveFinished(CCNode* sender);

	CCSprite * sprite;
};

