#pragma once
#include "d:\cocos2d-x\lazyguard\classes\tool.h"

class Fire :
	public Tool
{
public:
	Fire(void);
	~Fire(void);

	static Fire* fire();

	virtual bool init();
};
