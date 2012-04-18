#include "LevelEditor.h"
#include "Obstacle.h"
#include "AnimatePacker.h"


LevelEditor::LevelEditor(void)
{
}


LevelEditor::~LevelEditor(void)
{
// 	for (int i=0; i<mapWidth; i++)
// 	{
// 		delete[] obstacles[i];
// 	}
// 	delete[] obstacles;

	if (mapLayer)
	{
		mapLayer->release();
		mapLayer = NULL;
	}
	if (obstacles)
	{
		obstacles->release();
	}
	if (level)
	{
		level->release();
	}
}

CCScene* LevelEditor::scene()
{
	CCScene * scene = NULL;
	do 
	{
		// 'scene' is an autorelease object
		scene = CCScene::node();
		CC_BREAK_IF(! scene);

		// 'layer' is an autorelease object
		LevelEditor *layer = LevelEditor::node();
		CC_BREAK_IF(! layer);

		// add layer as a child to scene
		scene->addChild(layer);

	} while (0);

	// return the scene
	return scene;
}

bool LevelEditor::init()
{
	if( CCLayer::init() )
	{

		level = Level::level();
		level->retain();
		level->load();
		level->save();
		
		// map
		mapLayer = CCLayer::node();
		mapLayer->retain();
		mapLayer->setAnchorPoint(ccp(0,1));
		mapLayer->setScale(0.8);
		addChild(mapLayer);
		mapWidth = level->width;
		mapHeight = level->height;
		AnimatePacker::getInstance()->loadAnimate("sprites.xml");
		//background
 		char bacImg[16];
 		sprintf_s(bacImg, "back%d.png", level->background);
 		CCTexture2D *texture = CCTextureCache::sharedTextureCache()->addImage(bacImg);
 		ccTexParams tp = {GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT};
 		texture->setTexParameters(&tp);
		CCSprite* background = CCSprite::spriteWithTexture(texture);
 		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		background->setTextureRect(CCRectMake(0, 0, winSize.width, winSize.height));
 		background->setPosition(ccp(winSize.width/2, winSize.height/2));
 		mapLayer->addChild(background);
		// obstacle
		obstacles = CCArray::array();
		obstacles->retain();
//		obstacles = new int*[mapWidth];
// 		for (int i=0; i<mapWidth; i++)
// 		{
// 			obstacles[i] = new int[mapHeight];
// 			for (int j=0; j<mapHeight; j++)
// 			{
// 				obstacles[i][j] = 0;
// 			}
// 		}
		Thing* obsTemp;
		int obsType;
		int obsX;
		int obsY;
		CCPoint obsPos;
		CCMutableDictionary<std::string, CCString*>* propsTemp;
		for (int i=0; i<level->obstacles->count(); i++)
		{
			propsTemp = (CCMutableDictionary<std::string, CCString*>*)level->obstacles->objectAtIndex(i);
			obsType = propsTemp->objectForKey("type")->toInt();
			obsX = propsTemp->objectForKey("x")->toInt();
			obsY = propsTemp->objectForKey("y")->toInt();
			obsPos = ccp(obsX * level->tileWidth, obsY * level->tileHeight);
			obsTemp = Obstacle::obstacle(obsType, obsPos);
			obstacles->addObject(obsTemp);
			mapLayer->addChild(obsTemp);
		}





		setIsTouchEnabled(true);

		return true;
	}
	else
	{
		return false;
	}
}

void LevelEditor::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
	CCSetIterator it = touches->begin();
	CCTouch* touch = (CCTouch*)(*it);

	CCPoint m_tTouchPos;
	m_tTouchPos = mapLayer->convertTouchToNodeSpace(touch);
// 	m_tTouchPos = touch->locationInView( touch->view() );
// 	m_tTouchPos = CCDirector::sharedDirector()->convertToGL( m_tTouchPos );
	int tX = m_tTouchPos.x / level->tileWidth;
	int tY = m_tTouchPos.y / level->tileHeight;

	Obstacle* obsTemp;
	for (int i=0; i<obstacles->count(); i++)
	{
		obsTemp = (Obstacle*)obstacles->objectAtIndex(i);
		if (CCRect::CCRectContainsPoint(obsTemp->getRectOut(),m_tTouchPos))
		{
			obstacles->removeObject(obsTemp);
			mapLayer->removeChild(obsTemp, true);
		}
	}



// 	CCPoint obsPos = ccp(tX * level->tileWidth, tY * level->tileHeight);
// 	mapLayer->addChild(Obstacle::obstacle(1, obsPos));

}