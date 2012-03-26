#pragma once

#include "Character.h"
#include "Gem.h"

using namespace cocos2d;

class Thief :
	public Character
{
public:
	Thief(void);
	~Thief(void);

	static Thief* thief();
	virtual bool init();

	void findGem();
	void findHome();
	void moveFinished(CCNode* sender);
//  	void getGem(CCNode* sender);
	void fleeHome();
	void kill();
	void updateFrame(ccTime dt);
	bool inScreen();
// 	void updateTarget();

	static const int FINDING = 0, STEALING = 1, FLEEING = 2, BACKING = 3; 

	Gem* gem;
	float startX;
	float startY;
// 	bool isFleeing;
	int status;
	float findingInterval;
};
