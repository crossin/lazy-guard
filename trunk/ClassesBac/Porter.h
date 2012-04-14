#pragma once

#include "Character.h"
#include "Gem.h"

using namespace cocos2d;

class Porter :
	public Character
{
public:
	Porter(void);
	~Porter(void);

	static Porter* porter();
	virtual bool init();

	void findGem();
	void findHome();
	void patrol();
	void stun();
	void updateFrame(ccTime dt);

	//bool inScreen();
// 	void updateTarget();

	static const int PATROLING = 0, FINDING = 1, STUNNING = 2, BACKING = 3, WAITING = 4; 

	Gem* gem;
	//float startX;
	//float startY;
// 	bool isFleeing;
	int status;
	float findingInterval;
};
