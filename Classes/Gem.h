#pragma once

#include "Character.h"

using namespace cocos2d;

class Gem :
	public Character
{
public:
	Gem(void);
	~Gem(void);

	static Gem* gem();

	virtual bool init();
	
	void kill();

	//Thief* owner;
};

