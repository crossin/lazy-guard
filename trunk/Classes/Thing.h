#pragma once

#include "CCSprite.h"

using namespace cocos2d;

class Thing :
	public CCNode
{
public:
	Thing(void);
	~Thing(void);

	static const int GUARD = 1, THIEF = 2, PORTER = 3, ERASER = 4, OBSTACLE = 5, GEM = 6; 

	virtual bool init() = 0;

	CCRect getRectOut();

	CCSprite* sprite;
};

class Eraser :
	public Thing
{
public:
	bool init();
	static Eraser* eraser();
};