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
	if (buttonObs)
	{
		buttonObs->release();
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
		mapLayer->setAnchorPoint(ccp(0.2,0.7));
		mapLayer->setScale(0.7);
		addChild(mapLayer);
		//mapWidth = level->width;
		//mapHeight = level->height;
		AnimatePacker::getInstance()->loadAnimate("sprites.xml");
		//background
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		CCSprite* canvas = CCSprite::spriteWithFile("blank.png");
		canvas->setTextureRect(CCRectMake(0, 0, winSize.width+2*level->tileWidth, winSize.height+2*level->tileHeight));
		canvas->setPosition(ccp(winSize.width/2, winSize.height/2));
		mapLayer->addChild(canvas);

 		char bacImg[16];
 		sprintf_s(bacImg, "back%d.png", level->background);
 		CCTexture2D *texture = CCTextureCache::sharedTextureCache()->addImage(bacImg);
 		ccTexParams tp = {GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT};
 		texture->setTexParameters(&tp);
		background = CCSprite::spriteWithTexture(texture);
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
			mapLayer->addChild(obsTemp,20-obsY);
		}


		// menu
		layerObs = CCLayer::node();
		buttonObs = CCArray::array();
		buttonObs->retain();
		for (int i=0; i<4; i++)
		{
			obsPos = ccp(400+i%2*40, 260-i/2*50);
			obsTemp =  Obstacle::obstacle(i, obsPos);
			layerObs->addChild(obsTemp);
			buttonObs->addObject(obsTemp);
		}
		Eraser* eraser = Eraser::eraser();
		eraser->setPosition(ccp(400, 160));
		layerObs->addChild(eraser,0,Thing::ERASER);
		buttonObs->addObject(eraser);
		//mapLayer->retain();
		//mapLayer->setAnchorPoint(ccp(0.2,0.7));
		//mapLayer->setScale(0.7);
		addChild(layerObs);

		// the frame of select button
		frameSelect = CCSprite::spriteWithFile("blank.png");
		frameSelect->setAnchorPoint(ccp(0,0));
		
		//frameSelect->setPosition(ccp(440,300));
		//frameSelect->setColor(ccORANGE);
		frameSelect->setOpacity(127);
		frameSelect->setIsVisible(false);
		addChild(frameSelect);


		CCLabelTTF* label = CCLabelTTF::labelWithString("BUTTON", "Arial", 24);
		CCMenuItemLabel* pMenuItem = CCMenuItemLabel::itemWithLabel(label, this, menu_selector(LevelEditor::menuCallback));
		//m_pItmeMenu->addChild(pMenuItem);
		CCMenu* m_pItmeMenu = CCMenu::menuWithItems(pMenuItem, NULL);
		addChild(m_pItmeMenu);

		thingSelect = NULL;
		isEraser = false;

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
	m_tTouchPos = touch->locationInView( touch->view() );
	m_tTouchPos = CCDirector::sharedDirector()->convertToGL( m_tTouchPos );

layerObs->setIsVisible(false);

 	Thing* objectTemp;
// 	for (int i=0; i<obstacles->count(); i++)
// 	{
// 		obsTemp = (Obstacle*)obstacles->objectAtIndex(i);
// 		if (CCRect::CCRectContainsPoint(obsTemp->getRectOut(),m_tTouchPos))
// 		{
// 			obstacles->removeObject(obsTemp);
// 			mapLayer->removeChild(obsTemp, true);
// 		}
// 	}

	for (int i=0; i<5; i++)
	{

	 	objectTemp = (Thing*)buttonObs->objectAtIndex(i);
		CCRect a =objectTemp->getRectOut();
	 	if (CCRect::CCRectContainsPoint(objectTemp->getRectOut(),m_tTouchPos))
	 	{
			isEraser = (objectTemp->getTag() == Thing::ERASER);

			if (thingSelect)
			{
				thingSelect->sprite->setColor(ccWHITE);
			}
			thingSelect = objectTemp;
			//typeSelect = obsTemp->typeIndex;
			thingSelect->sprite->setColor(ccGRAY/*ccc3(122,122,255)*/);

// 			frameSelect->setTextureRect(obsTemp->getRectOut());
// 			frameSelect->setPosition(obsTemp->getPosition());
// 			frameSelect->setIsVisible(true);
			return;
	 	}
	}

	if (thingSelect)
	{
		m_tTouchPos = mapLayer->convertTouchToNodeSpace(touch);
		int tX = m_tTouchPos.x / level->tileWidth;
		int tY = m_tTouchPos.y / level->tileHeight;
		if (CCRect::CCRectContainsPoint(background->boundingBox(),m_tTouchPos))
		{
			CCPoint obsPos = ccp(tX * level->tileWidth, tY * level->tileHeight);
			mapLayer->addChild(Obstacle::obstacle(((Obstacle*)thingSelect)->typeIndex, obsPos), 20-tY);
		}
	}

}

void LevelEditor::menuCallback(CCObject * pSender)
{
	layerObs->setIsVisible(false);
}