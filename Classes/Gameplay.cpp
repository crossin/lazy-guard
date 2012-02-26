#include "Gameplay.h"
#include "aStarLibrary.h"
#include "ccMacros.h"

using namespace cocos2d;

int game_map[32][48];
/*={
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,1,1,1,0,0,0,0},
	{0,0,0,1,0,0,0,0,0,1,0,0},
	{0,0,0,1,0,0,0,0,0,1,0,0},
	{0,0,0,1,0,0,-1,0,0,1,0,0},
	{0,0,0,1,0,0,0,0,0,1,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0}
};*/

int r;
int c;
int w;

char textout[8];
CCLabelTTF* pLabel;
CCSprite *player;
/*
Gameplay::Gameplay(void)
{
}

Gameplay::~Gameplay(void)
{
}
*/

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
		player->setPosition(ccp(size.width/2, size.height/2));

		//// Add the sprite to HelloWorld layer as a child layer.
		this->addChild(player);



		bRet = true;
	} while (0);

	setIsTouchEnabled(true);

	// init map
	r = (sizeof(game_map)/sizeof(game_map[0]));
	c = (sizeof(game_map[0])/sizeof(game_map[0][0]));
	for (int i=0;i<r;i++){
		for (int j=0;j<c;j++){
			if (CCRANDOM_0_1() < 0.15){
				game_map[i][j] = 1;
				walkability [j][i] = unwalkable;
			}
		}
	}
	w = 480/c;
	_itoa_s(w,textout,10);
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

	

	//// draw a simple line
	//// The default state is:
	//// Line Width: 1
	//// color: 255,255,255,255 (white, non-transparent)
	//// Anti-Aliased
	//glEnable(GL_LINE_SMOOTH);
	//ccDrawLine( CCPointMake(0, 0), CCPointMake(s.width, s.height) );
	//
	//// line: color, width, aliased
	//// glLineWidth > 1 and GL_LINE_SMOOTH are not compatible
	//// GL_SMOOTH_LINE_WIDTH_RANGE = (1,1) on iPhone
	//glDisable(GL_LINE_SMOOTH);
	//glLineWidth( 5.0f );
	///*glColor4ub(255,0,0,255);*/
	//glColor4f(1.0, 0.0, 0.0, 1.0);
	//ccDrawLine( CCPointMake(0, s.height), CCPointMake(s.width, 0) );

	//// TIP:
	//// If you are going to use always the same color or width, you don't
	//// need to call it before every draw
	////
	//// Remember: OpenGL is a state-machine.
	//
	//// draw big point in the center
	//glPointSize(64);
	///*glColor4ub(0,0,255,128);*/
	//glColor4f(0.0, 0.0, 1.0, 0.5);
	//ccDrawPoint( CCPointMake(s.width / 2, s.height / 2) );
	//
	//// draw 4 small points
	//CCPoint points[] = { CCPointMake(60,60), CCPointMake(70,70), CCPointMake(60,70), CCPointMake(70,60) };
	//glPointSize(4);
	///*glColor4ub(0,255,255,255);*/
	//glColor4f(0.0, 1.0, 1.0, 1.0);
	//ccDrawPoints( points, 4);
	//
	//// draw a green circle with 10 segments
	//glLineWidth(16);
	///*glColor4ub(0, 255, 0, 255);*/
	//glColor4f(0.0, 1.0, 0.0, 1.0);
	//ccDrawCircle( CCPointMake(s.width/2,  s.height/2), 100, 0, 10, false);

	//// draw a green circle with 50 segments with line to center
	//glLineWidth(2);
	///*glColor4ub(0, 255, 255, 255);*/
	//glColor4f(0.0, 1.0, 1.0, 1.0);
	//ccDrawCircle( CCPointMake(s.width/2, s.height/2), 50, CC_DEGREES_TO_RADIANS(90), 50, true);	
	//
	//// open yellow poly
	///*glColor4ub(255, 255, 0, 255);*/
	//glColor4f(1.0, 1.0, 0.0, 1.0);
	//glLineWidth(10);
	//CCPoint vertices[] = { CCPointMake(0,0), CCPointMake(50,50), CCPointMake(100,50), CCPointMake(100,100), CCPointMake(50,100) };
	//ccDrawPoly( vertices, 5, false);
	//
	//// closed purble poly
	///*glColor4ub(255, 0, 255, 255);*/
	//glColor4f(1.0, 0.0, 1.0, 1.0);
	//glLineWidth(2);
	//CCPoint vertices2[] = { CCPointMake(30,130), CCPointMake(30,230), CCPointMake(50,200) };
	//ccDrawPoly( vertices2, 3, true);
	//
	//// draw quad bezier path
	//ccDrawQuadBezier(CCPointMake(0,s.height), CCPointMake(s.width/2,s.height/2), CCPointMake(s.width,s.height), 50);

	//// draw cubic bezier path
	//ccDrawCubicBezier(CCPointMake(s.width/2, s.height/2), CCPointMake(s.width/2+30,s.height/2+50), CCPointMake(s.width/2+60,s.height/2-50),CCPointMake(s.width, s.height/2),100);

	//
	//// restore original values
	//glLineWidth(1);
	///*glColor4ub(255,255,255,255);*/
	//glColor4f(1.0, 1.0, 1.0, 1.0);
	//glPointSize(1); 

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

	FindPath(1,m_tTouchPos.x,m_tTouchPos.y,240,160);

	//_itoa_s((*(pathBank[1]-4)),textout,10);
	//itoa(pathLength[1],textout,10);
	for (int i=0;i<pathLength[1];i++)
	{
		game_map[pathBank[1][2*i+1]][pathBank[1][2*i]] = 2;
	}
	//pLabel->setString(textout);

	player->setPosition(m_tTouchPos);
}