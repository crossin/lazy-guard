#pragma once
#include "cocos2d.h"
#include "Level.h"

using namespace cocos2d;

class LevelEditor :
	public CCLayer
{
public:
	LevelEditor(void);
	~LevelEditor(void);

	static CCScene* scene();

	SCENE_NODE_FUNC(LevelEditor);
	CC_SYNTHESIZE_READONLY(CCLayer*, mapLayer, Map);

	bool init();
	void ccTouchesEnded(CCSet* touches, CCEvent* event);

	Level *level;
	CCArray* obstacles;
//	int** obstacles;
	int mapWidth;
	int mapHeight;
};

