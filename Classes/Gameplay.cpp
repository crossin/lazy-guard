// #define _CRTDBG_MAP_ALLOC 
// #include<stdlib.h> 
// #include<crtdbg.h> 

// #include <vld.h>

#include "Gameplay.h"
#include "ccMacros.h"
#include "CCMutableDictionary.h"
#include "PathFinder.h"
#include <stdlib.h>


using namespace cocos2d;

int game_map[20][30];
//PathFinder *pathfinder;

int r;
int c;
int w;

char textout[8];
CCLabelTTF* pLabel;
CCSprite *player;

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

		r = (sizeof(game_map)/sizeof(game_map[0]));
		c = (sizeof(game_map[0])/sizeof(game_map[0][0]));
		w = 480/c;

		//_itoa_s(pDesertTileMap->getTileSize().height,textout,10);
		//pathfinder = new PathFinder(pDesertTileMap->getMapSize().width, pDesertTileMap->getMapSize().height, pDesertTileMap->getTileSize().width, pDesertTileMap->getTileSize().height);
		PathFinder* pathfinder = PathFinder::getInstance();
		pathfinder->initWithSize(pDesertTileMap->getMapSize().width, pDesertTileMap->getMapSize().height, pDesertTileMap->getTileSize().width, pDesertTileMap->getTileSize().height);

		int tileGID;
		CCDictionary<std::string, CCString*>* props;
		CCString* colli;
		for (int i=0;i<r;i++){
			for (int j=0;j<c;j++){
				if (tileGID = meta->tileGIDAt(ccp(j,r-i-1)))
				{
					if (props = pDesertTileMap->propertiesForGID(tileGID))
					{
						colli = props->objectForKey("Collidable");
						if (colli->m_sString.compare("True") == 0)
						{
							game_map[i][j] = 1;
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

		// Create a label and initialize with string "Hello World".
		pLabel = CCLabelTTF::labelWithString("Hello World", "Thonburi", 64);
		CC_BREAK_IF(! pLabel);

		// Get window size and place the label upper. 
		CCSize size = CCDirector::sharedDirector()->getWinSize();
		pLabel->setPosition(ccp(size.width / 2, size.height - 20));

		// Add the label to HelloWorld layer as a child layer.
		this->addChild(pLabel, 1);

		//// 3. Add add a splash screen, show the cocos2d splash image.
		player = CCSprite::spriteWithFile("Player.png");
		//CC_BREAK_IF(! pSprite);

		//// Place the sprite on the center of the screen
		player->setAnchorPoint(ccp(0,0));//(ccp(player->getContentSize().width/2,player->getContentSize().height/2));
		player->setPosition(ccp(size.width/2, size.height/2));

		//// Add the sprite to HelloWorld layer as a child layer.
		this->addChild(player);

		thieves = new CCMutableArray<Thief*>;
		addThief();
		addThief();
		addThief();

		bRet = true;
	} while (0);

	setIsTouchEnabled(true);

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
	
	
	pLabel->setString(textout);

	return bRet;
}

void Gameplay::draw()
{
	CCLayer::draw();
	CCSize s = CCDirector::sharedDirector()->getWinSize();
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
}

void Gameplay::FindThief() 
{

	Thief* closest = thieves->getLastObject();

	// check caught
	CCRect rectG = CCRectMake(player->getPosition().x,player->getPosition().y,player->getContentSize().width,player->getContentSize().height);
	CCRect rectT = CCRectMake(closest->getPosition().x,closest->getPosition().y,closest->getContentSize().width,closest->getContentSize().height);

	if (CCRect::CCRectIntersectsRect(rectG,rectT))
	{
		return;
	}

	PathFinder* pathfinder = PathFinder::getInstance();
	pathfinder->FindPath(1,player->getPosition().x,player->getPosition().y,closest->getPosition().x,closest->getPosition().y);

	//_itoa_s((*(pathBank[1]-4)),textout,10);
	//itoa(pathLength[1],textout,10);
	CCArray* path = CCArray::array();
	CCPoint target, from;
	CCPoint moveDifference;
	float distanceToMove;
	float moveDuration;
	CCFiniteTimeAction* actionMove;

	if (pathfinder->pathLength<2)
	{
		return;
	}
//	game_map[pathBank[1][3]][pathBank[1][2]] = 2;
	from = ccp(pathfinder->pathBank[0] * w, pathfinder->pathBank[1] * w);
	target = ccp(pathfinder->pathBank[2] * w, pathfinder->pathBank[3] * w);
	moveDifference = ccpSub(target,from);
	distanceToMove = ccpLength(moveDifference);
	moveDuration = distanceToMove/100;
	actionMove = CCMoveTo::actionWithDuration((ccTime)moveDuration, target);

	CCFiniteTimeAction* actionMoveDone = CCCallFuncN::actionWithTarget( this, callfuncN_selector(Gameplay::spriteMoveFinished));

	//CCFiniteTimeAction* actionMoveDone = CCCallFuncN::actionWithTarget( this, callfuncN_selector(HelloWorld::spriteMoveFinished));
	player->runAction( CCSequence::actions(actionMove,actionMoveDone,NULL) );
}

void Gameplay::spriteMoveFinished(CCNode* sender)
{
	CCSprite *sprite = (CCSprite *)sender;
	this->FindThief();
}

void Gameplay::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
	//clear
	for (int i=0;i<r;i++){
		for (int j=0;j<c;j++){
			if (game_map[i][j] == 2)
			{
				game_map[i][j] = 0;
			}
		}
	}

	CCSetIterator it = touches->begin();
	CCTouch* touch = (CCTouch*)(*it);

	CCPoint m_tTouchPos;
	m_tTouchPos = touch->locationInView( touch->view() );	
	m_tTouchPos = CCDirector::sharedDirector()->convertToGL( m_tTouchPos );

	game_map[int(m_tTouchPos.y / w)][int(m_tTouchPos.x / w)] = 2;
	
	FindThief();



	

}

void Gameplay::addThief()
{
	Thief *thief = Thief::thief();
	thieves->addObject(thief);
	addChild(thief);	
	thief->findPath();
}


