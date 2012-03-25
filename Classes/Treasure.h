#pragma once

#include "cocos2d.h"
#include "CCGeometry.h"

using namespace cocos2d;

class Treasure :
	public CCNode
{
public:
	Treasure(void);
	~Treasure(void);

	static Treasure* treasure(int count, CCPoint pos);

	bool init(int count, CCPoint pos);

	CCArray* gems;
};

