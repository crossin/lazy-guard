#pragma once

#include "Thing.h"

using namespace cocos2d;

class Obstacle :
	public Thing
{
public:
	Obstacle(void);
	~Obstacle(void);

	static Obstacle* obstacle(int t, CCPoint pos);

	virtual bool init();
	bool init(int t, CCPoint pos);

	int typeIndex;
};
