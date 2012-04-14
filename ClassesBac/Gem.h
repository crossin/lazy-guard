#pragma once

#include "Thing.h"

using namespace cocos2d;

class Gem :
	public Thing
{
public:
	Gem(void);
	~Gem(void);

	static Gem* gem();

	virtual bool init();
	void kill();

};

