#pragma once
#include "cocos2d.h"
#include "Level.h"
#include "Thing.h"

using namespace cocos2d;

class LevelEditor :
	public CCLayer
{
public:
	LevelEditor(void);
	~LevelEditor(void);

	static CCScene* scene();

	bool init();
	void ccTouchesEnded(CCSet* touches, CCEvent* event);
	void menuCallback(CCObject * pSender);
	void editObstacle(CCPoint posTouch, CCPoint posInMap);
	bool save();
	SCENE_NODE_FUNC(LevelEditor);
	CC_SYNTHESIZE_READONLY(CCLayer*, mapLayer, Map);

	Level *level;
	CCLayer* layerObs;
	CCLayer* layerThief;
	CCArray* obstacles;
	CCArray* buttonObs;
	CCArray* buttonThief;
	CCSprite* background;
	//CCSprite* frameSelect;
//	int** obstacles;
	//int mapWidth;
	//int mapHeight;
	Thing* thingSelect;
	bool isEraser;
	int status;
	//int typeSelect;
};

