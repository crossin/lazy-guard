#ifndef _GAMEPLAY_H_
#define _GAMEPLAY_H_

#include "cocos2d.h"
#include "Thief.h"
#include "Guard.h"
#include "Gem.h"
#include "Treasure.h"
#include "Porter.h"
#include "Obstacle.h"

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

	//void updateThieves();

virtual void draw();
 //   void menuCallback(CCObject * pSender);
 //   void closeCallback(CCObject * pSender);
	//virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
 //   virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
	CCMutableArray<Thief*>* thieves;
	CCMutableArray<Thing*>* things;
	CCMutableArray<Guard*>* guards;
	CCMutableArray<Obstacle*>* obstacles;
	Porter* porter;
	CCArray* gemsOutside;
	CCArray* thievesPool;
	Treasure* treasure;
	//int countThief;
	int countGem;
	float timeLife;
	CCMenuItem* toolSelected;
CCLabelTTF* pLabel;

private:
	//void addThief();
	void addThief(ccTime dt);
	void updateFrame(ccTime dt);
	//     void FindThief();
// 	void spriteMoveFinished(CCNode* sender);
	//CCMenu* m_pItmeMenu;
	virtual void keyBackClicked();
	void overlapped(Thing* t1, Thing* t2);
	void caughtThief(Guard* gd, Thief* tf);
	void robbedPorter(Thief* tf, Porter* pt);
	void thiefGotGem(Thief* tf);
	void porterGotGem(Porter* pt);
	void btnClicked(CCObject * pSender);
	void useClock(CCPoint posTouch);
	void useTorch(CCPoint posTouch);
	void useBomb(CCPoint posTouch);
};

#endif

