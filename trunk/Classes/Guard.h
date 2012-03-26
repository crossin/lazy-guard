#pragma once

#include "Character.h"

using namespace cocos2d;

class Guard :
	public Character
{
public:
	Guard(void);
	~Guard(void);

	static Guard* guard();
	virtual bool init();

	void findThief();
	//void spriteMoveFinished(CCNode* sender);
	void onHit();
	void updateFrame(ccTime dt);
	void setAwake(bool w);
	void patrol();
	CCRect getRectClick();
	static const int SLEEPING = 0, WAKING = 1, CHASING = 2, PATROLING = 3; 

	CCSprite * bar;
	//bool isAwake;
	int status;
	float pointSleep;
	float pointSleepMax;
	float pointWake;
	float pointWakeMax;
	int range; 
	float findingInterval;
	//CCAction* actionWalk;
};

