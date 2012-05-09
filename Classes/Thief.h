#pragma once

#include "Character.h"
#include "Gem.h"
#include "Clock.h"

using namespace cocos2d;

class Thief :
	public Character
{
public:
	Thief(void);
	~Thief(void);

	static Thief* thief(int ts);
	virtual bool init();
	bool init(int ts);

	void findGem();
	void findHome();
	void moveFinished();
//  	void getGem(CCNode* sender);
	void fleeHome();
	void runWithFire();
	void kill();
	void updateFrame(ccTime dt);
	//void checkStart(ccTime dt);
	bool inScreen();
	void setClock( bool on );
	void setFire( Fire* fr );

	// 	void updateTarget();

	static const int FINDING = 0, FLEEING = 1, BACKING = 2, BURNING = 3;

	Gem* gem;
	float startX;
	float startY;
 	bool hasVisited;
	int status;
	float findingInterval;
	int timeStart;
	Clock* clock;
	Fire* fire;
	bool inAction;
	//bool onFire;
	//float timeLife;
};
