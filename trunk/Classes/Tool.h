#pragma once
#include "Thing.h"
using namespace cocos2d;

class Tool :
	public Thing
{
public:
	Tool(void);
	~Tool(void);

	virtual bool init() = 0;

	static const int CLOCK = 1, TORCH = 2, BOMB = 3;

	float lifetime;
};
