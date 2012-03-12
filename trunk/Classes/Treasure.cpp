#include "Treasure.h"
#include "Gem.h"

Treasure::Treasure(void)
{
}


Treasure::~Treasure(void)
{
	if (gems)
	{
		gems->release();
		gems = NULL;
	}
}

Treasure* Treasure::treasure(int count, int ix, int iy)
{
	Treasure *treasure = new Treasure;
	if (treasure && treasure->init(count, ix, iy))
	{
		treasure->autorelease();
		return treasure;
	}

	return NULL;
}

bool Treasure::init(int count, int ix, int iy)
{
	bool bRet = false;
	do{
		this->setAnchorPoint(CCPointZero);
		posX = ix;
		posY = iy;

		gems = CCArray::arrayWithCapacity(count);
		gems->retain();
		for (int i = 0; i < count; i++)
		{
			Gem* g = Gem::gem();
			g->setPosition(ccp(posX+8*sin(i*6.28/5), posY+8*cos(i*6.28/5)));
			this->addChild(g);
			gems->addObject(g);
		}

		bRet=true;
	}while(0);

	return bRet;
}
