#pragma once

#include "CCObject.h"

using namespace cocos2d;

class Level : public CCObject
{
public:
	Level(void);
	~Level(void);

	static Level* level();
	bool init();

	bool save();
};
