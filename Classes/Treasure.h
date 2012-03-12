#pragma once

#include "cocos2d.h"

using namespace cocos2d;

class Treasure :
	public CCNode
{
public:
	Treasure(void);
	~Treasure(void);

	static Treasure* treasure(int count, int ix, int iy);

	bool init(int count, int ix, int iy);

	CCArray* gems;
	int posX;
	int posY;
};

