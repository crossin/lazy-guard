#ifndef _GAMEPLAY_H_
#define _GAMEPLAY_H_

#include "cocos2d.h"

using namespace cocos2d;

class Gameplay : public CCLayer
{
public:
	Gameplay(void);
	~Gameplay(void);

	static cocos2d::CCScene* scene();

	virtual bool init();  
	virtual void draw();

	LAYER_NODE_FUNC(Gameplay);

	void ccTouchesEnded(CCSet* touches, CCEvent* event);

 //   void menuCallback(CCObject * pSender);
 //   void closeCallback(CCObject * pSender);

	//virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
 //   virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
	CCMutableArray<CCSprite*> *thieves;
private:
	void addThief();
    //CCPoint m_tBeginPos;
    //CCMenu* m_pItmeMenu;
};

#endif

