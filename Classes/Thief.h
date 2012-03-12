#pragma once

#include "CCSprite.h"
#include "Gem.h"

using namespace cocos2d;

class Thief :
	public CCNode
{
public:
	Thief(void);
	~Thief(void);

	static Thief* thief();
	bool init();

	void findGem();
	void findHome();
	void moveFinished(CCNode* sender);
	void getGem(CCNode* sender);
	void kill();
	CCRect getRect();
	void fleeHome();
	void updateFrame(ccTime dt);

	CCSprite * sprite;
	Gem* gem;
	float startX;
	float startY;
	bool isFleeing;
	int speed;
};
