#include "LevelEditor.h"
#include "Obstacle.h"
#include "Thief.h"
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
	if (buttonThief)
	{
		buttonThief->release();
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
		Thing* objTemp;
		int objType;
		int objX;
		int objY;
		CCPoint objPos;
		CCMutableDictionary<std::string, CCString*>* propsTemp;
		for (int i=0; i<level->obstacles->count(); i++)
		{
			propsTemp = (CCMutableDictionary<std::string, CCString*>*)level->obstacles->objectAtIndex(i);
			objType = propsTemp->objectForKey("type")->toInt();
			objX = propsTemp->objectForKey("x")->toInt();
			objY = propsTemp->objectForKey("y")->toInt();
			objPos = ccp(objX * level->tileWidth, objY * level->tileHeight);
			objTemp = Obstacle::obstacle(objType, objPos);
			obstacles->addObject(objTemp);
			mapLayer->addChild(objTemp,20-objY);
		}


		// button
		// obstacle
		layerObs = CCLayer::node();
		buttonObs = CCArray::array();
		buttonObs->retain();
		for (int i=0; i<4; i++)
		{
			objPos = ccp(400+i%2*40, 260-i/2*50);
			objTemp =  Obstacle::obstacle(i, objPos);
			layerObs->addChild(objTemp);
			buttonObs->addObject(objTemp);
		}
		Eraser* eraser = Eraser::eraser();
		eraser->setPosition(ccp(400, 160));
		buttonObs->addObject(eraser);
		layerObs->addChild(eraser,0,Thing::ERASER);
		//layerObs->setIsVisible(false);
		addChild(layerObs);

		// thief
		layerThief = CCLayer::node();
		buttonThief = CCArray::array();
		buttonThief->retain();
// 		for (int i=0; i<1; i++)
// 		{
// 			obsPos = ccp(400+i%2*40, 260-i/2*50);
// 			obsTemp =  Obstacle::obstacle(i, obsPos);
// 			layerObs->addChild(obsTemp);
// 			buttonObs->addObject(obsTemp);
// 		}
		objPos = ccp(415, 260);
		objTemp = Thief::thief(0);
		objTemp->unscheduleAllSelectors();
		objTemp->setPosition(objPos);
		layerThief->addChild(objTemp);
		buttonThief->addObject(objTemp);
		eraser = Eraser::eraser();
		eraser->setPosition(ccp(440, 260));
		buttonThief->addObject(eraser);
		layerThief->addChild(eraser,0,Thing::ERASER);


		//thief start time
		startTime = (SGText*)CCTextFieldTTF::textFieldWithPlaceHolder("InputHere","Thonburi",50);
		//設定顏色
		startTime->setColor(ccc3(255,0,0));
		//設定位置
		startTime->setPosition(ccp(415,260));
		startTime->setDelegate(startTime);
		//開啟文字輸入框
		//startTime->attachWithIME();

		layerThief->addChild(startTime);

		layerThief->setIsVisible(false);
		addChild(layerThief);
		//startTime->detachWithIME();


		// menu
		char* menu_name[2] = {"Obstacle", "Thief"};
		int menu_tag[2] = {Thing::OBSTACLE, Thing::THIEF};
		CCLabelTTF* label;
		CCMenuItemLabel* pMenuItem;
		CCMenu* m_pItmeMenu = CCMenu::menuWithItems(NULL);
		for (int i = 0; i < 2; i++)
		{
			label = CCLabelTTF::labelWithString(menu_name[i], "Arial", 24);
			pMenuItem = CCMenuItemLabel::itemWithLabel(label, this, menu_selector(LevelEditor::menuCallback));
			m_pItmeMenu->addChild(pMenuItem, 0, menu_tag[i]);
		}
		m_pItmeMenu->alignItemsHorizontallyWithPadding(10);
		m_pItmeMenu->setPosition(ccp(100,20));
		addChild(m_pItmeMenu);








		thingSelect = NULL;
		isEraser = false;
		status = Thing::OBSTACLE;

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
//////////////////////////////////////////////////////////////////////////
CCLOG("%s",startTime->getString());


	CCSetIterator it = touches->begin();
	CCTouch* touch = (CCTouch*)(*it);

	CCPoint m_tTouchPos;
	m_tTouchPos = touch->locationInView( touch->view() );
	m_tTouchPos = CCDirector::sharedDirector()->convertToGL( m_tTouchPos );

 	CCPoint m_tTouchPosInMap = mapLayer->convertTouchToNodeSpace(touch);


	switch (status)
	{
	case Thing::OBSTACLE:
		editObstacle(m_tTouchPos, m_tTouchPosInMap);
	}

///////////////////////////////
save();
///////////////////////////////
}

void LevelEditor::editObstacle(CCPoint posTouch, CCPoint posInMap)
{
	Thing* objectTemp;
	for (int i=0; i<5; i++)
	{
		objectTemp = (Thing*)buttonObs->objectAtIndex(i);
		CCRect a =objectTemp->getRectOut();
		if (CCRect::CCRectContainsPoint(objectTemp->getRectOut(), posTouch))
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
		int tX = posInMap.x / level->tileWidth;
		int tY = posInMap.y / level->tileHeight;
		if (CCRect::CCRectContainsPoint(background->boundingBox(), posInMap))
		{
			CCPoint obsPos = ccp(tX * level->tileWidth, tY * level->tileHeight);
			// check if hit obstacles
			for (int i=0; i<obstacles->count(); i++)
			{
				objectTemp = (Obstacle*)obstacles->objectAtIndex(i);
				if (CCPoint::CCPointEqualToPoint(objectTemp->getPosition(), obsPos))
				{
					if (isEraser)
					{
						obstacles->removeObject(objectTemp);
						mapLayer->removeChild(objectTemp, true);
					}
					return;
				}
			}
			if (!isEraser)
			{
				objectTemp = Obstacle::obstacle(((Obstacle*)thingSelect)->typeIndex, obsPos);
				obstacles->addObject(objectTemp);
				mapLayer->addChild(objectTemp, 20-tY);
			}
		}
	}
}

void LevelEditor::menuCallback(CCObject * pSender)
{
	layerObs->setIsVisible(false);
	layerThief->setIsVisible(false);
	status = ((CCMenuItem*)pSender)->getTag();
	CCLayer* layer;
	switch (status)
	{
	case Thing::OBSTACLE:
		layer = layerObs;
		break;
	case Thing::THIEF:
		layer = layerThief;
		break;
	}
	layer->setIsVisible(true);
}

bool LevelEditor::save()
{
	level->obstacles->removeAllObjects();
	CCMutableDictionary<std::string, CCString*>* dic;
	Obstacle* obsTemp;
	char number[4];
	CCString* str;
	int len;
	for (int i=0; i<obstacles->count(); i++)
	{
		obsTemp = (Obstacle*)obstacles->objectAtIndex(i);
		dic = new CCMutableDictionary<std::string, CCString*>();
		//type
		sprintf_s(number, "%d", obsTemp->typeIndex);
		str = new CCString(number);
		dic->setObject(str, "type");
		//x
		len = (int)obsTemp->getPosition().x / level->tileWidth;
		sprintf_s(number, "%d", len);
		str = new CCString(number);
		dic->setObject(str, "x");
		//y
		len = (int)obsTemp->getPosition().y / level->tileHeight;
		sprintf_s(number, "%d", len);
		str = new CCString(number);
		dic->setObject(str, "y");

		level->obstacles->addObject(dic);
		str->release();
		dic->release();
	}
	return level->save();
}

// SGText
bool SGText::onTextFieldAttachWithIME(CCTextFieldTTF * pSender)
{	
	//setColor(ccc3(255,255,255));
	return true;
}

bool SGText::onTextFieldDetachWithIME(CCTextFieldTTF * pSender)
{	
	setColor(ccc3(255,255,255));
	return true;
}