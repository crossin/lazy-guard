#include "Treasure.h"
#include "Gameplay.h"
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

Treasure* Treasure::treasure(int count, CCPoint pos)
{
	Treasure *treasure = new Treasure;
	if (treasure && treasure->init(count, pos))
	{
		treasure->autorelease();
		return treasure;
	}

	return NULL;
}

bool Treasure::init(int count, CCPoint pos)
{
	bool bRet = false;
	do{
		setPosition(ccp(pos.x, pos.y));
		gems = CCArray::arrayWithCapacity(count);
		gems->retain();
		for (int i = 0; i < count; i++)
		{
			Gem* g = Gem::gem();
			g->setPosition(ccp(pos.x+8*sin(i*6.28/5), pos.y+8*cos(i*6.28/5)));
			gems->addObject(g);
		}
		bRet=true;
	}while(0);

	return bRet;
}
