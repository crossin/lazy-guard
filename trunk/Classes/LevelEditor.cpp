#include "LevelEditor.h"
#include "Obstacle.h"
#include "Thief.h"
#include "Guard.h"
#include "Gem.h"
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
	if (thieves)
	{
		thieves->release();
	}
	if (buttonObs)
	{
		buttonObs->release();
	}
	if (buttonThief)
	{
		buttonThief->release();
	}
	if (buttonGuard)
	{
		buttonGuard->release();
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
		//level->save();
		
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
		canvas = CCSprite::spriteWithFile("blank.png");
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
		int objParam;
		int objX;
		int objY;
		CCPoint objPos;
		CCMutableDictionary<std::string, CCString*>* propsTemp;
		for (int i=0; i<level->obstacles->count(); i++)
		{
			propsTemp = (CCMutableDictionary<std::string, CCString*>*)level->obstacles->objectAtIndex(i);
			objParam = propsTemp->objectForKey("type")->toInt();
			objX = propsTemp->objectForKey("x")->toInt();
			objY = propsTemp->objectForKey("y")->toInt();
			objPos = ccp(objX * level->tileWidth, objY * level->tileHeight);
			objTemp = Obstacle::obstacle(objParam, objPos);
			obstacles->addObject(objTemp);
			mapLayer->addChild(objTemp,20-objY);
		}
		//Thief
		thieves = CCArray::array();
		thieves->retain();

		for (int i=0; i<level->thieves->count(); i++)
		{
			propsTemp = (CCMutableDictionary<std::string, CCString*>*)level->thieves->objectAtIndex(i);
			objParam = propsTemp->objectForKey("time")->toInt();
			objX = propsTemp->objectForKey("x")->toInt();
			objY = propsTemp->objectForKey("y")->toInt();
			//objPos = ccp(objX * level->tileWidth, objY * level->tileHeight);
			objTemp = Thief::thief(objParam);
			objTemp->unscheduleAllSelectors();
			objTemp->setPosition(ccp(objX,objY));
			thieves->addObject(objTemp);
			mapLayer->addChild(objTemp,340-objY);
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

		textBack = CCSprite::spriteWithFile("blank.png");
		textBack->setTextureRect(CCRectMake(0, 0, 80, 20));
		textBack->setPosition(ccp(434, 304));
		layerThief->addChild(textBack);

		buttonThief = CCArray::array();
		buttonThief->retain();
		objPos = ccp(415, 240);
		objTemp = Thief::thief(0);
		objTemp->setScale(1.5);
		objTemp->unscheduleAllSelectors();
		objTemp->setPosition(objPos);
		layerThief->addChild(objTemp);
		buttonThief->addObject(objTemp);
		eraser = Eraser::eraser();
		eraser->setPosition(ccp(440, 240));
		buttonThief->addObject(eraser);
		layerThief->addChild(eraser,0,Thing::ERASER);

		//thief start time
		startTime = CCTextFieldTTF::textFieldWithPlaceHolder("","Thonburi",20);
		startTime->setColor(ccBLACK);
		startTime->setString("0");
		startTime->setPosition(ccp(434,304));
		layerThief->addChild(startTime);

		layerThief->setIsVisible(false);
		addChild(layerThief);

		// guard
		layerGuard = CCLayer::node();
		buttonGuard = CCArray::array();
		buttonGuard->retain();
		objPos = ccp(415, 260);
		objTemp = Guard::guard();
		objTemp->unscheduleAllSelectors();
		objTemp->setPosition(objPos);
		layerGuard->addChild(objTemp);
		buttonGuard->addObject(objTemp);
		eraser = Eraser::eraser();
		eraser->setPosition(ccp(440, 260));
		buttonGuard->addObject(eraser);
		layerGuard->addChild(eraser,0,Thing::ERASER);
		layerGuard->setIsVisible(false);
		addChild(layerGuard);

		// gem
		layerGem = CCLayer::node();
		objPos = ccp(415, 280);
		objTemp = Gem::gem();
		objTemp->setScale(2);
		objTemp->setPosition(objPos);
		layerGem->addChild(objTemp);
		buttonGem = objTemp;
		layerGem->setIsVisible(false);
		addChild(layerGem);

		// menu
		char* menu_name[4] = {"Obstacle", "Thief", "Guard", "Gem"};
		int menu_tag[4] = {Thing::OBSTACLE, Thing::THIEF, Thing::GUARD, Thing::GEM};
		CCLabelTTF* label;
		CCMenuItemLabel* pMenuItem;
		CCMenu* m_pItmeMenu = CCMenu::menuWithItems(NULL);
		for (int i = 0; i < 4; i++)
		{
			label = CCLabelTTF::labelWithString(menu_name[i], "Arial", 24);
			pMenuItem = CCMenuItemLabel::itemWithLabel(label, this, menu_selector(LevelEditor::menuCallback));
			m_pItmeMenu->addChild(pMenuItem, 0, menu_tag[i]);
		}
		m_pItmeMenu->alignItemsHorizontallyWithPadding(20);
		m_pItmeMenu->setPosition(ccp(200,20));
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
// CCLOG("%s",startTime->getString());


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
		break;
	case Thing::THIEF:
		editThief(m_tTouchPos, m_tTouchPosInMap);
		break;
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

void LevelEditor::editThief(CCPoint posTouch, CCPoint posInMap)
{
	if (CCRect::CCRectContainsPoint(textBack->boundingBox(), posTouch))
	{
		startTime->attachWithIME();
		startTime->setString("");
	}
	else
	{
		startTime->detachWithIME();
	}

	Thing* objectTemp;
	for (int i=0; i<2; i++)
	{
		objectTemp = (Thing*)buttonThief->objectAtIndex(i);
		if (CCRect::CCRectContainsPoint(objectTemp->getRectOut(), posTouch))
		{
			isEraser = (objectTemp->getTag() == Thing::ERASER);

			if (thingSelect)
			{
				thingSelect->sprite->setColor(ccWHITE);
			}
			thingSelect = objectTemp;
			thingSelect->sprite->setColor(ccGRAY);
			return;
		}
	}

	if (thingSelect)
	{
		CCString* str;
		//int tX = posInMap.x / level->tileWidth;
		//int tY = posInMap.y / level->tileHeight;
		if (CCRect::CCRectContainsPoint(canvas->boundingBox(), posInMap)
			&& !CCRect::CCRectContainsPoint(background->boundingBox(), posInMap))
		{
			posInMap.x = posInMap.x<0 ? -16 : (posInMap.x>480 ? 496 : posInMap.x);
			posInMap.y = posInMap.y<0 ? -32 : (posInMap.y>320 ? 320 : posInMap.y);

			//CCPoint obsPos = ccp(tX * level->tileWidth, tY * level->tileHeight);
			if (!isEraser)
			{
				str = new CCString(startTime->getString());
				objectTemp = Thief::thief(str->toInt());
				objectTemp->unscheduleAllSelectors();
				objectTemp->setPosition(posInMap);
				thieves->addObject(objectTemp);
				mapLayer->addChild(objectTemp, 340-posInMap.y);
				str->release();
			}
			else
			{
				for (int i=thieves->count()-1; i>=0; i--)
				{
					objectTemp = (Thief*)thieves->objectAtIndex(i);
					if (CCRect::CCRectContainsPoint(objectTemp->getRectOut(), posInMap))
					{
						thieves->removeObject(objectTemp);
						mapLayer->removeChild(objectTemp, true);
						return;
					}
				}
			}
		}
	}
}

void LevelEditor::menuCallback(CCObject * pSender)
{
	if (status == ((CCMenuItem*)pSender)->getTag())
	{
		return;
	}
	status = ((CCMenuItem*)pSender)->getTag();
	layerObs->setIsVisible(false);
	layerThief->setIsVisible(false);
	layerGuard->setIsVisible(false);
	layerGem->setIsVisible(false);
	isEraser = false;
	if (thingSelect)
	{
		thingSelect->sprite->setColor(ccWHITE);
		thingSelect = NULL;
	}
	CCLayer* layer;
	switch (status)
	{
	case Thing::OBSTACLE:
	default:
		layer = layerObs;
		break;
	case Thing::THIEF:
		layer = layerThief;
		break;
	case Thing::GUARD:
		layer = layerGuard;
		break;
	case Thing::GEM:
		layer = layerGem;
		break;
	}
	layer->setIsVisible(true);
}

bool LevelEditor::save()
{
	//obstacle
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
		str->release();
		//x
		len = (int)obsTemp->getPosition().x / level->tileWidth;
		sprintf_s(number, "%d", len);
		str = new CCString(number);
		dic->setObject(str, "x");
		str->release();
		//y
		len = (int)obsTemp->getPosition().y / level->tileHeight;
		sprintf_s(number, "%d", len);
		str = new CCString(number);
		dic->setObject(str, "y");
		str->release();

		level->obstacles->addObject(dic);
		dic->release();
	}
	//thief
	level->thieves->removeAllObjects();
	Thief* tfTemp;
	for (int i=0; i<thieves->count(); i++)
	{
		tfTemp = (Thief*)thieves->objectAtIndex(i);
		dic = new CCMutableDictionary<std::string, CCString*>();
		//time
		sprintf_s(number, "%d", tfTemp->timeStart);
		str = new CCString(number);
		dic->setObject(str, "time");
		str->release();
		//x
		len = (int)tfTemp->getPosition().x;
		sprintf_s(number, "%d", len);
		str = new CCString(number);
		dic->setObject(str, "x");
		str->release();
		//y
		len = (int)tfTemp->getPosition().y;
		sprintf_s(number, "%d", len);
		str = new CCString(number);
		dic->setObject(str, "y");
		str->release();

		level->thieves->addObject(dic);
		dic->release();
	}
	return level->save();
}

// bool LevelEditor::onTextFieldAttachWithIME(CCTextFieldTTF * pSender)
// {	
//	//startTime->setColor(ccc3(255,0,0));
//	//startTime->setString("");
//	return false;
//}
//
//bool LevelEditor::onTextFieldDetachWithIME(CCTextFieldTTF * pSender)
//{	
//	//startTime->setColor(ccc3(255,255,255));
//	return false;
//}