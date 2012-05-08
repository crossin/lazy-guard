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
	void runWithFire();
	void setClock(bool on);
	void setFire(bool on);
	void setBomb(CCPoint bPos);
	CCRect getRectClick();
	void stunOver();
	static const int SLEEPING = 0, WAITING = 1, CHASING = 2, PATROLING = 3, BURNING = 4, STUNNING = 5; 

	CCSprite * bar;
	//bool isAwake;
	int status;
	float pointSleep;
	float pointSleepMax;
	float pointWake;
	float pointWakeMax;
	int range; 
	float findingInterval;
	bool onClock;
	bool inAction;
	//int numClock;
	//CCAction* actionWalk;
};

