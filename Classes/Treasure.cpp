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
		setPosition(pos);
		gems = CCArray::arrayWithCapacity(count);
		gems->retain();
		for (int i = 0; i < count; i++)
		{
			Gem* g = Gem::gem();
			setGemPos(g, i);
			gems->addObject(g);
		}
		bRet=true;
	}while(0);

	return bRet;
}

void Treasure::setGemPos(Gem* g, int idx)
{

	g->setPosition(ccp(getPosition().x+8*sin(idx*6.28/5), getPosition().y+8*cos(idx*6.28/5)));
}
