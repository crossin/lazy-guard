#pragma once
#include "Tool.h"
#include "Character.h"
#include "Obstacle.h"

class Fire :
	public Tool
{
public:
	Fire(void);
	~Fire(void);

	static Fire* fire();

	virtual bool init();
	void update(ccTime dt);
	void kill();

	Character* owner;
	Obstacle* terrain;
	float spreadInterval;
	float INTERVAL;
};
