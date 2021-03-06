#pragma once
#include "Tool.h"

class Bomb :
	public Tool
{
public:
	Bomb(void);
	~Bomb(void);

	static Bomb* bomb();

	virtual bool init();
	void update(ccTime dt);
	void kill();

};

class Flame :
	public CCSprite
{
public:
	static Flame* flame();
	void kill();
};

