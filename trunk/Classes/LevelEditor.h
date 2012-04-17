#pragma once
#include "cocos2d.h"
#include "Level.h"

using namespace cocos2d;

class LevelEditor :
	public CCScene
{
public:
	LevelEditor(void);
	~LevelEditor(void);

	bool init();
	SCENE_NODE_FUNC(LevelEditor);
	CC_SYNTHESIZE_READONLY(CCLayer*, mapLayer, Map);

	Level *level;
};

