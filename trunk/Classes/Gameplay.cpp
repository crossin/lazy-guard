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
#include "AnimatePacker.h"
#include "Obstacle.h"


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
	if (things)
	{
		things->release();
		things = NULL;
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
		
// 		CCTMXLayer *meta=pDesertTileMap->layerNamed("Meta");
// 		meta->setIsVisible(false);
// 		CCTMXLayer *layerGem = pDesertTileMap->layerNamed("gem");
// 		layerGem->setIsVisible(false);
		CCTMXLayer *layerObstacle = pDesertTileMap->layerNamed("Things");
		layerObstacle->setIsVisible(false);
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

		AnimatePacker::getInstance()->loadAnimate("sprites.xml");
		things = new CCMutableArray<Thing*>;

		int tileGID;
		CCDictionary<std::string, CCString*>* props;
		CCString* result;
		CCPoint posTemp;
/*
		for (int i=0;i<r;i++){
			for (int j=0;j<c;j++){
				pathfinder->setUnwalkable(j, i, false);
				if (tileGID = meta->tileGIDAt(ccp(j,r-i-1)))
				{
					if (props = pDesertTileMap->propertiesForGID(tileGID))
					{
						result = props->objectForKey("Collidable");
						if (result->m_sString.compare("True") == 0)
						{
							game_map[i][j] = 1;
							//pathfinder->walkability [j][i] = pathfinder->unwalkable;
							pathfinder->setUnwalkable(j, i, true);
						}
					}
				}
			
				//if ( ){
				//	game_map[i][j] = 1;
				//	
				//}
			}
		}
*/
		Thing* obsTemp;
		for (int i=0;i<r;i++){
			for (int j=0;j<c;j++){
				pathfinder->setUnwalkable(j, i, false);
				if (tileGID = layerObstacle->tileGIDAt(ccp(j,r-i-1)))
				{
					if (props = pDesertTileMap->propertiesForGID(tileGID))
					{
						result = props->objectForKey("Type");
						posTemp = ccp((j+0.5)*pDesertTileMap->getTileSize().width, (i+0.5)*pDesertTileMap->getTileSize().height);
						obsTemp = Obstacle::obstacle(result->toInt(), posTemp);
						pathfinder->setUnwalkable(j, i, true);
						addChild(obsTemp);
						things->addObject(obsTemp);
// 						if (result->m_sString.compare("True") == 0)
// 						{
// 							game_map[i][j] = 1;
// 							//pathfinder->walkability [j][i] = pathfinder->unwalkable;
// 							pathfinder->setUnwalkable(j, i, true);
// 						}
					}
				}
			}
		}


		// 2. Add a label shows "Hello World".

		// Create a label and initialize with string "Hello World".
		pLabel = CCLabelTTF::labelWithString("10", "Thonburi", 64);
		CC_BREAK_IF(! pLabel);

		// Get window size and place the label upper. 
		CCSize size = CCDirector::sharedDirector()->getWinSize();
		pLabel->setPosition(ccp(size.width / 2, size.height - 20));

		// Add the label to HelloWorld layer as a child layer.
		this->addChild(pLabel, 1);

		//// 3. Add add a splash screen, show the cocos2d splash image.
		

		//gem
		CCTMXObjectGroup *objects = pDesertTileMap->objectGroupNamed("Objects");
		props = objects->objectNamed("Treasure");
		posTemp = ccp(props->objectForKey("x")->toInt(), props->objectForKey("y")->toInt());
		countGem = props->objectForKey("Count")->toInt();

// 		for (int i=0;i<r;i++){
// 			for (int j=0;j<c;j++){
// 				if (tileGID = layerGem->tileGIDAt(ccp(j,r-i-1)))
// 				{
// 					if (props = pDesertTileMap->propertiesForGID(tileGID))
// 					{
// 						result = props->objectForKey("count");
// 						countGem = result->toInt();
// 						posTemp = ccp((j+0.5)*pDesertTileMap->getTileSize().width, (i+0.5)*pDesertTileMap->getTileSize().height);
// 					}
// 				}
// 			}
// 		}
		treasure = Treasure::treasure(countGem, posTemp);
		addChild(treasure);
		Gem* g;
		for (int i=0; i<countGem; i++)
		{
			g = (Gem*)treasure->gems->objectAtIndex(i);
			addChild(g, 0, Thing::GEM);
			things->addObject(g);
		}
		gemsOutside = CCArray::arrayWithCapacity(countGem);
		gemsOutside->retain();

		//guard
		guard[0] = Guard::guard();
		guard[0]->setPosition(ccp(100,100));
		addChild(guard[0], 1, Thing::GUARD);
		addChild(guard[0]->bar, 1001);
		things->addObject(guard[0]);

		guard[1] = Guard::guard();
		guard[1]->setPosition(ccp(300,200));
		addChild(guard[1], 1, Thing::GUARD);
		addChild(guard[1]->bar, 1001);
		things->addObject(guard[1]);
		
		//thief
		thieves = new CCMutableArray<Thief*>;
		countThief = 20;

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

//clear
PathFinder* pathfinder = PathFinder::getInstance();
for (int i=0;i<10;i++){
	for (int j=0;j<15;j++){
		if (pathfinder->walkability[j][i] == PathFinder::unwalkable)
		{
			game_map[i][j] = 1;
		}
	}
}

	CCSetIterator it = touches->begin();
	CCTouch* touch = (CCTouch*)(*it);

	CCPoint m_tTouchPos;
	m_tTouchPos = touch->locationInView( touch->view() );
	m_tTouchPos = CCDirector::sharedDirector()->convertToGL( m_tTouchPos );

	//game_map[int(m_tTouchPos.y / w)][int(m_tTouchPos.x / w)] = 2;
// 	CCRect rectGuard;
	for (int i = 0; i < 2; i++)
	{
// 		rectGuard = CCRectMake(guard[i]->getPosition().x, guard[i]->getPosition().y, guard[i]->sprite->getContentSize().width, guard[i]->sprite->getContentSize().height);
		if (!guard[i]->status != Guard::SLEEPING && CCRect::CCRectContainsPoint(guard[i]->getRectClick(), m_tTouchPos))
		{
			guard[i]->onHit();
			break;
// 			guard[i]->isAwake = true;
// 			guard[i]->findThief();
		}
	}
}

void Gameplay::addThief()
{
	Thief *thief = Thief::thief();
	thieves->addObject(thief);
	addChild(thief, 1, Thing::THIEF);
	things->addObject(thief);
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
	CCMutableArray<Thing*>::CCMutableArrayIterator it1;
	CCMutableArray<Thing*>::CCMutableArrayIterator it2;
	for (it1 = things->begin(); it1 != things->end(); it1++)
	{
		for (it2 = things->begin(); it2 != things->end(); it2++)
		{
			overlapped(*it1, *it2);
		}
	}

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

void Gameplay::overlapped(Thing* t1, Thing* t2)
{
	if (t1 == t2)
	{
		return;
	}

	// guard vs. thief
	if (t1->getTag() == Thing::GUARD && t2->getTag() == Thing::THIEF)
	{
		Guard* gd = (Guard*)t1;
		Thief* tf = (Thief*)t2;
		if (tf->status != Thief::FLEEING  && tf->inScreen()
			&& gd->status != Guard::SLEEPING && ccpDistance(gd->getPosition(), tf->getPosition()) < gd->range)
		{
			if (tf->gem)
			{
				reorderChild(tf->gem, 0);
				gemsOutside->addObject(tf->gem);
				tf->gem = NULL;
			}
			tf->fleeHome();
			gd->stopAllActions();
		}				
	}

	// thief vs. gem
	if (t1->getTag() == Thing::THIEF && t2->getTag() == Thing::GEM)
	{
		Thief* tf = (Thief*)t1;
		Gem* gm = (Gem*)t2;

		if (tf->status == Thief::FINDING || tf->status == Thief::BACKING)
		{
			if (CCRect::CCRectIntersectsRect(tf->getRect(), gm->getRect()))
			{
				// outside
				if (gemsOutside->containsObject(gm))
				{
					reorderChild(gm, 1000);
					gemsOutside->removeObject(g);
					tf->gem = gm;
					tf->status = Thief::STEALING;
					tf->findHome();
				}
				// treasure
				if (treasure->gems->containsObject(gm))
				{
				}
			}
		}


		//CCArray* gemsList = ((Gameplay*)getParent())->gemsOutside;
		// 	rectThief = CCRectMake(this->getPosition().x, this->getPosition().y, this->sprite->getContentSize().width, this->sprite->getContentSize().height);

		// in treasure
		Treasure* trs = ((Gameplay*)getParent())->treasure;
		if (gem == NULL && CCRect::CCRectContainsPoint(this->getRect(), trs->getPosition()))
		{
			status = BACKING;
			gemsList = trs->gems;
			if (gem = (Gem*)gemsList->lastObject())
			{
				((Gameplay*)getParent())->reorderChild(gem, 1000);
				gemsList->removeLastObject();
				status = STEALING;
			}
			this->findHome();
		}
	}
	
	// check z-order
	if (t1->getTag() == Thing::GEM || t2->getTag() == Thing::GEM)
	{
		return;
	}
	if (CCRect::CCRectIntersectsRect(t1->getRect(), t2->getRect()))
	{
		if (t1->getPosition().y > t2->getPosition().y && t1->getZOrder() >= t2->getZOrder())
		{
			reorderChild(t2, t1->getZOrder()+1);
		}
	}

}

void Gameplay::updateThieves()
{
	CCMutableArray<Thief*>::CCMutableArrayIterator it;
	Thief* thief;
	for (it = thieves->begin(); it != thieves->end(); it++)
	{
		thief = *it;
// 		thief->updateTarget();
		// update action
		if (thief->status == Thief::FINDING || thief->status == Thief::BACKING)
		{
			thief->findGem();
		}
	}
}

void Gameplay::keyBackClicked()
{
	CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}