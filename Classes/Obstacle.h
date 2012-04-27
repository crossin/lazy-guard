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
	static const int TREE = 0, WOOD = 1, ROCK = 2, STEEL = 3; 

	virtual bool init();
	bool init(int t, CCPoint pos);

	int typeIndex;
};
