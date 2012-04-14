#pragma once

#include "cocos2d.h"

using namespace cocos2d;

class Level : public CCObject
{
public:
	Level(void);
	~Level(void);

	static Level* level();
	bool init();

	bool save();
	bool load();
	CCArray* obstacles;
	int width;
	int height;
	int tileWidth;
	int tileHeight;
	int background;
	//CCMutableDictionary<std::string, CCString*>* props;
};
