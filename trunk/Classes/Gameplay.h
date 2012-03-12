#ifndef _GAMEPLAY_H_
#define _GAMEPLAY_H_

#include "cocos2d.h"
#include "Thief.h"
#include "Guard.h"
#include "Gem.h"
#include "Treasure.h"

using namespace cocos2d;

class Gameplay : public CCLayer
{
public:
	Gameplay(void);
	~Gameplay(void);

	static cocos2d::CCScene* scene();

	virtual bool init();  


	LAYER_NODE_FUNC(Gameplay);

	void ccTouchesEnded(CCSet* touches, CCEvent* event);

virtual void draw();
 //   void menuCallback(CCObject * pSender);
 //   void closeCallback(CCObject * pSender);
	//virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
 //   virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
	CCMutableArray<Thief*>* thieves;
	Guard* guard[2];
	//CCArray* gems;
	Treasure* treasure;
	int countThief;
	int countGem;

private:
	void addThief();
	void gameLogic(ccTime dt);
	void updateFrame(ccTime dt);
	//     void FindThief();
// 	void spriteMoveFinished(CCNode* sender);
	//CCMenu* m_pItmeMenu;
	virtual void keyBackClicked();
};

#endif

