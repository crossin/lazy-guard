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
 	CCRect getRect();
	void onHit();
	void updateFrame(ccTime dt);
	void setAwake(bool w);
	void patrol();

	static const int SLEEPING = 0, WAKING = 1, CHASING = 2, PATROLING = 3; 

	CCSprite * sprite;
	CCSprite * bar;
	//bool isAwake;
	int status;
	float pointSleep;
	float pointSleepMax;
	float pointWake;
	float pointWakeMax;
	int speed;
	float speedRot;
	int range;
	CCAction* actionWalk;
};

