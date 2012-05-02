#pragma once
#include "Tool.h"
#include "Character.h"

class Clock :
	public Tool
{
public:
	Clock(void);
	~Clock(void);

	static Clock* clock();

	virtual bool init();
	void update(ccTime dt);
	void kill();

	Character* owner;
	
};
