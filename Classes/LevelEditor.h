#pragma once
#include "cocos2d.h"
#include "Level.h"

using namespace cocos2d;

class MapLayer : public CCLayer
{
public:
	LAYER_NODE_FUNC(MapLayer);
	void ccTouchesEnded(CCSet* touches, CCEvent* event);
};

class LevelEditor :
	public CCScene
{
public:
	LevelEditor(void);
	~LevelEditor(void);

	bool init();
	SCENE_NODE_FUNC(LevelEditor);
	CC_SYNTHESIZE_READONLY(MapLayer*, mapLayer, Map);

	Level *level;
	CCArray* obstacles;
//	int** obstacles;
	int mapWidth;
	int mapHeight;
};

