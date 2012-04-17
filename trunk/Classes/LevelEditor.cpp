#include "LevelEditor.h"
#include "Obstacle.h"
#include "AnimatePacker.h"


LevelEditor::LevelEditor(void)
{
}


LevelEditor::~LevelEditor(void)
{
	if (mapLayer)
	{
		mapLayer->release();
		mapLayer = NULL;
	}
}

bool LevelEditor::init()
{
	if( CCScene::init() )
	{

		level = Level::level();
		level->load();
		level->save();
		
		// map
		mapLayer = CCLayer::node();
		mapLayer->retain();
		mapLayer->setAnchorPoint(ccp(0,1));
		mapLayer->setScale(0.8);
		addChild(mapLayer);

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
			mapLayer->addChild(obsTemp);
		}








		return true;
	}
	else
	{
		return false;
	}
}
