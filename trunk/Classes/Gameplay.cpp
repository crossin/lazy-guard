// #define _CRTDBG_MAP_ALLOC 
// #include<stdlib.h> 
// #include<crtdbg.h> 

// #include <vld.h>

#include "Gameplay.h"
#include "ccMacros.h"
#include "CCMutableDictionary.h"
#include "PathFinder.h"
#include "GameOverScene.h"
//#include <stdlib.h>
#include <math.h>


using namespace cocos2d;


int game_map[10][15];
/*
//PathFinder *pathfinder;
int r;
int c;
int w;

char textout[8];
CCLabelTTF* pLabel;
//CCSprite *player;
*/

Gameplay::Gameplay(void):thieves(NULL)
{
}

Gameplay::~Gameplay(void)
{
	if (thieves)
	{
		thieves->release();
		thieves = NULL;
	}
	if (gemsOutside)
	{
		gemsOutside->release();
		gemsOutside = NULL;
	}
	PathFinder::release();

	//delete pathfinder;

	//_CrtDumpMemoryLeaks();
}


CCScene* Gameplay::scene()
{
	CCScene * scene = NULL;
	do 
	{
		// 'scene' is an autorelease object
		scene = CCScene::node();
		CC_BREAK_IF(! scene);

		// 'layer' is an autorelease object
		Gameplay *layer = Gameplay::node();
		CC_BREAK_IF(! layer);

		// add layer as a child to scene
		scene->addChild(layer);

	} while (0);

	// return the scene
	return scene;
}

bool Gameplay::init()
{
	bool bRet = false;
	srand(time(NULL));
	do 
	{
		//////////////////////////////////////////////////////////////////////////
		// super init first
		//////////////////////////////////////////////////////////////////////////

		CC_BREAK_IF(! CCLayer::init());

		//////////////////////////////////////////////////////////////////////////
		// add your codes below...
		//////////////////////////////////////////////////////////////////////////
		
		// load the tile map
		CCTMXTiledMap *pDesertTileMap = CCTMXTiledMap::tiledMapWithTMXFile("background.tmx");
		pDesertTileMap->setPosition(ccp(0,0));
		addChild(pDesertTileMap, 0, 1);
		
		CCTMXLayer *meta=pDesertTileMap->layerNamed("Meta");
		meta->setIsVisible(false);
		CCTMXLayer *layerGem = pDesertTileMap->layerNamed("gem");
		layerGem->setIsVisible(false);
/*
		r = (sizeof(game_map)/sizeof(game_map[0]));
		c = (sizeof(game_map[0])/sizeof(game_map[0][0]));
		w = 480/c;
*/
		int r = pDesertTileMap->getMapSize().height;
		int c = pDesertTileMap->getMapSize().width;

		//_itoa_s(pDesertTileMap->getTileSize().height,textout,10);
		//pathfinder = new PathFinder(pDesertTileMap->getMapSize().width, pDesertTileMap->getMapSize().height, pDesertTileMap->getTileSize().width, pDesertTileMap->getTileSize().height);
		PathFinder* pathfinder = PathFinder::getInstance();
		pathfinder->initWithSize(pDesertTileMap->getMapSize().width, pDesertTileMap->getMapSize().height, pDesertTileMap->getTileSize().width, pDesertTileMap->getTileSize().height);

		int tileGID;
		CCDictionary<std::string, CCString*>* props;
		CCString* result;

		for (int i=0;i<r;i++){
			for (int j=0;j<c;j++){
				if (tileGID = meta->tileGIDAt(ccp(j,r-i-1)))
				{
					if (props = pDesertTileMap->propertiesForGID(tileGID))
					{
						result = props->objectForKey("Collidable");
						if (result->m_sString.compare("True") == 0)
						{
							//game_map[i][j] = 1;
							//pathfinder->walkability [j][i] = pathfinder->unwalkable;
							pathfinder->setUnwalkable(j, i);
						}
					}
				}
			
				//if ( ){
				//	game_map[i][j] = 1;
				//	
				//}
			}
		}

		// 2. Add a label shows "Hello World".
/*
		// Create a label and initialize with string "Hello World".
		pLabel = CCLabelTTF::labelWithString("10", "Thonburi", 64);
		CC_BREAK_IF(! pLabel);

		// Get window size and place the label upper. 
		CCSize size = CCDirector::sharedDirector()->getWinSize();
		pLabel->setPosition(ccp(size.width / 2, size.height - 20));

		// Add the label to HelloWorld layer as a child layer.
		//this->addChild(pLabel, 1);
*/
		//// 3. Add add a splash screen, show the cocos2d splash image.
		
		
		//gem
// 		countGem = 5;
// 		gems = CCArray::arrayWithCapacity(5);
// 		gems->retain();
// 		for (int i = 0; i < 5; i++)
// 		{
// 			Gem* g = Gem::gem();
// 			g->setPosition(ccp(240+8*sin(i*6.28/5),160+8*cos(i*6.28/5)));
// 			addChild(g, 1000);
// 			gems->addObject(g);
// 		}
		CCPoint posGem;
		for (int i=0;i<r;i++){
			for (int j=0;j<c;j++){
				if (tileGID = layerGem->tileGIDAt(ccp(j,r-i-1)))
				{
					if (props = pDesertTileMap->propertiesForGID(tileGID))
					{
						result = props->objectForKey("count");
						countGem = result->toInt();
						posGem = ccp((j+0.5)*pDesertTileMap->getTileSize().width, (i+0.5)*pDesertTileMap->getTileSize().height);
					}
				}
			}
		}
		treasure = Treasure::treasure(countGem, posGem.x, posGem.y);
		this->addChild(treasure);
		gemsOutside = CCArray::arrayWithCapacity(countGem);
		gemsOutside->retain();
		//guard
		guard[0] = Guard::guard();
		guard[0]->setPosition(ccp(100,100));
		this->addChild(guard[0], 1001);
		guard[1] = Guard::guard();
		guard[1]->setPosition(ccp(300,200));
		this->addChild(guard[1], 1002);
		//thief
		thieves = new CCMutableArray<Thief*>;
		countThief = 10;

		bRet = true;
	} while (0);

	setIsTouchEnabled(true);
	setIsKeypadEnabled(true);

	schedule( schedule_selector(Gameplay::gameLogic), 3 );
	schedule( schedule_selector(Gameplay::updateFrame));

	// init map
	//r = (sizeof(game_map)/sizeof(game_map[0]));
	//c = (sizeof(game_map[0])/sizeof(game_map[0][0]));
	//for (int i=0;i<r;i++){
	//	for (int j=0;j<c;j++){
	//		if (CCRANDOM_0_1() < 0.15){
	//			game_map[i][j] = 1;
	//			walkability [j][i] = unwalkable;
	//		}
	//	}
	//}
	
	
	//pLabel->setString(textout);

	return bRet;
}


void Gameplay::draw()
{
	CCLayer::draw();
	CCSize s = CCDirector::sharedDirector()->getWinSize();
	int w = 32;
	int r = 10;
	int c = 15;
	glPointSize(w);
	glColor4f(0.4f, 0.4f, 0.4f, 1.0f);
	for (int i=0;i<r;i++){
		for (int j=0;j<c;j++){
			switch(game_map[i][j]){
				case -1:
					glColor4f(0.9f, 0.4f, 0.4f, 1.0f);
					break;
				case 0:
					glColor4f(0.4f, 0.4f, 0.4f, 1.0f);
					break;
				case 1:
					glColor4f(0.1f, 0.1f, 0.1f, 1.0f);
					break;
				case 2:
					glColor4f(0.9f, 0.9f, 0.4f, 1.0f);
					break;
			}	
			ccDrawPoint( CCPointMake((j+0.5f) * w, (i+0.5f) * w ));
		}
	}
// 	CCPoint vertices[4];
// 	if (thieves->count()>2)
// 	{
// 		CCRect r = thieves->getObjectAtIndex(1)->getRect();
// 		//CCRect r = guard->getRect();
// 		vertices[0]=ccp(CCRect::CCRectGetMinX(r),CCRect::CCRectGetMinY(r));
// 		vertices[1]=ccp(CCRect::CCRectGetMaxX(r),CCRect::CCRectGetMinY(r));
// 		vertices[2]=ccp(CCRect::CCRectGetMaxX(r),CCRect::CCRectGetMaxY(r));
// 		vertices[3]=ccp(CCRect::CCRectGetMinX(r),CCRect::CCRectGetMaxY(r));
// 		glPointSize(5);
// 		glColor4f(1,1,1,1);
// 		ccDrawPoly(vertices,4,true,false);
// 	}
}


// void Gameplay::FindThief() 
// {
// 
// 	Thief* closest = thieves->getLastObject();
// 
// 	// check caught
// 	CCRect rectG = CCRectMake(player->getPosition().x,player->getPosition().y,player->getContentSize().width,player->getContentSize().height);
// 	CCRect rectT = CCRectMake(closest->getPosition().x,closest->getPosition().y,closest->getContentSize().width,closest->getContentSize().height);
// 
// 	if (CCRect::CCRectIntersectsRect(rectG,rectT))
// 	{
// 		return;
// 	}
// 
// 	PathFinder* pathfinder = PathFinder::getInstance();
// 	pathfinder->FindPath(1,player->getPosition().x,player->getPosition().y,closest->getPosition().x,closest->getPosition().y);
// 
// 	//_itoa_s((*(pathBank[1]-4)),textout,10);
// 	//itoa(pathLength[1],textout,10);
// 	CCArray* path = CCArray::array();
// 	CCPoint target, from;
// 	CCPoint moveDifference;
// 	float distanceToMove;
// 	float moveDuration;
// 	CCFiniteTimeAction* actionMove;
// 
// 	if (pathfinder->pathLength<2)
// 	{
// 		return;
// 	}
// //	game_map[pathBank[1][3]][pathBank[1][2]] = 2;
// 	from = ccp(pathfinder->pathBank[0] * w, pathfinder->pathBank[1] * w);
// 	target = ccp(pathfinder->pathBank[2] * w, pathfinder->pathBank[3] * w);
// 	moveDifference = ccpSub(target,from);
// 	distanceToMove = ccpLength(moveDifference);
// 	moveDuration = distanceToMove/100;
// 	actionMove = CCMoveTo::actionWithDuration((ccTime)moveDuration, target);
// 
// 	CCFiniteTimeAction* actionMoveDone = CCCallFuncN::actionWithTarget( this, callfuncN_selector(Gameplay::spriteMoveFinished));
// 
// 	//CCFiniteTimeAction* actionMoveDone = CCCallFuncN::actionWithTarget( this, callfuncN_selector(HelloWorld::spriteMoveFinished));
// 	player->runAction( CCSequence::actions(actionMove,actionMoveDone,NULL) );
// }

// void Gameplay::spriteMoveFinished(CCNode* sender)
// {
// 	CCSprite *sprite = (CCSprite *)sender;
// 	this->FindThief();
// }

void Gameplay::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
/*
	//clear
	for (int i=0;i<r;i++){
		for (int j=0;j<c;j++){
			if (game_map[i][j] == 2)
			{
				game_map[i][j] = 0;
			}
		}
	}
*/
	CCSetIterator it = touches->begin();
	CCTouch* touch = (CCTouch*)(*it);

	CCPoint m_tTouchPos;
	m_tTouchPos = touch->locationInView( touch->view() );
	m_tTouchPos = CCDirector::sharedDirector()->convertToGL( m_tTouchPos );

	//game_map[int(m_tTouchPos.y / w)][int(m_tTouchPos.x / w)] = 2;
	CCRect rectGuard;
	for (int i = 0; i < 2; i++)
	{
		rectGuard = CCRectMake(guard[i]->getPosition().x, guard[i]->getPosition().y, guard[i]->sprite->getContentSize().width, guard[i]->sprite->getContentSize().height);
		if (!guard[i]->status != Guard::SLEEPING && CCRect::CCRectContainsPoint(rectGuard, m_tTouchPos))
		{
			guard[i]->onHit();
// 			guard[i]->isAwake = true;
// 			guard[i]->findThief();
		}
	}
	
}

void Gameplay::addThief()
{
	Thief *thief = Thief::thief();
	thieves->addObject(thief);
	addChild(thief);	
	thief->findGem();

// 	char a[10];
// 	sprintf(a,"%d",thieves->count());
// 	OutputDebugStringA(a);
}

void Gameplay::gameLogic(ccTime dt)
{
	if (countThief > 0)
	{
		addThief();
		countThief--;
		//_itoa_s(countThief,textout,10);
		//pLabel->setString(textout);
	}
}

void Gameplay::updateFrame(ccTime dt)
{
	//CCMutableArray<Thief*> *thievesToDelete = new CCMutableArray<Thief*>;
	CCMutableArray<Thief*>::CCMutableArrayIterator it;
	Thief* thief;

	for (it = thieves->begin(); it != thieves->end(); it++)
	{
		thief = *it;
		// check caught
		if (!thief->isFleeing && thief->inScreen())
		{
			for (int i = 0; i < 2; i++)
			{
				//if (guard[i]->isAwake && CCRect::CCRectIntersectsRect(guard[i]->getRect(), thief->getRect()))
				if (guard[i]->status != Guard::SLEEPING && ccpDistance(guard[i]->getPosition(), thief->getPosition()) < guard[i]->range)
				{
					if (thief->gem)
					{
						//treasure->gems->addObject(thief->gem);
						//int i = treasure->gems->indexOfObject(thief->gem);
						//thief->gem->setPosition(ccp(treasure->posX+8*sin(i*6.28/5),treasure->posY+8*cos(i*6.28/5)));
						this->reorderChild(thief->gem, 0);
						gemsOutside->addObject(thief->gem);
						thief->gem = NULL;
					}
					//thievesToDelete->addObject(thief);
					thief->fleeHome();
					guard[i]->stopAllActions();
					//guard[i]->isAwake = false;
				}
			}
		}
	}


// 	for (it = thievesToDelete->begin(); it != thievesToDelete->end(); it++ )
// 	{
// 		thief = *it;
// 		thief->kill();
// 	}

	// check win
	if (countThief == 0 && thieves->count() == 0)
	{
		GameOverScene *gameOverScene = GameOverScene::node();
		gameOverScene->getLayer()->getLabel()->setString("You Win!");
		CCDirector::sharedDirector()->replaceScene(gameOverScene);
	}

	// check lose
	if (countGem == 0)
	{
		GameOverScene *gameOverScene = GameOverScene::node();
		gameOverScene->getLayer()->getLabel()->setString("You Lose!");
		CCDirector::sharedDirector()->replaceScene(gameOverScene);
	}
}

void Gameplay::keyBackClicked()
{
	CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}