#ifndef _GAMEPLAY_H_
#define _GAMEPLAY_H_

#include "cocos2d.h"

using namespace cocos2d;

class Gameplay : public CCLayer
{
public:
	Gameplay(void);
	~Gameplay(void);

	virtual void draw();

	void ccTouchesEnded(CCSet* touches, CCEvent* event);

 //   void menuCallback(CCObject * pSender);
 //   void closeCallback(CCObject * pSender);

	//virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
 //   virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);

private:
    //CCPoint m_tBeginPos;
    //CCMenu* m_pItmeMenu;
};

#endif

