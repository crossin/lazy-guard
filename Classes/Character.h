#pragma once

#include "Thing.h"
#include "CCAction.h"
#include "Fire.h"
#include "Clock.h"

using namespace cocos2d;

class Fire;
class Clock;

class Character :
	public Thing
{
public:
	Character(void);
	~Character(void);

	static const float INTERVAL;

	virtual bool init() = 0;
	virtual void setClock(Clock* clk);
	virtual void setFire(Fire* fr);
	virtual void setBomb(CCPoint bPos);
	virtual void stunOver() = 0;
	CCFiniteTimeAction* makeAction(CCPoint from, CCPoint target);
	void changeDirection(CCNode *node, void *param);
	CCRect getRectIn();
	void runWithFire();
	int speed;
	float speedFactor;
	CCFiniteTimeAction* actionWalks[8];
	CCFiniteTimeAction* actionWalk;
	CCSpeed* actionSpeed;
	float findingInterval;
	int status;
	bool inAction;
	bool onBomb;
	Fire* fire;
	Clock* clock;
};
