/*
;===================================================================
;Modified on A* Pathfinder (Version 1.71a) by Patrick Lester. 
;===================================================================
 2012-03-01
 */

#pragma once
#ifndef _PATHFINDING_H_
#define _PATHFINDING_H_

#include "Singleton.h"

class PathFinder : public Singleton<PathFinder>
{
public:
	PathFinder(void);
	~PathFinder(void);

	bool initWithSize(int mWeight, int mHeight, int tWeight, int tHeight);
	void ReadPath(int currentX,int currentY, int pixelsPerFrame);
	int ReadPathX(int pathLocation);
	int ReadPathY(int pathLocation);

	void InitializePathfinder (void);
	void EndPathfinder (void);
	int FindPath (int startingX, int startingY, int targetX, int targetY);
	void setUnwalkable(int gridX, int gridY);

	//Declare constants
	static const int notfinished = 0, notStarted = 0;// path-related constants
	static const int found = 1, nonexistent = 2;//, same = 3; 
	static const int walkable = 0, unwalkable = 1;// walkability array constants

	int mapWidth;
	int mapHeight;
	int tileWidth;
	int tileHeight;
	int onClosedList;

	//Create needed arrays
	char** walkability;
	int* openList; //1 dimensional array holding ID# of open list items
	int** whichList;  //2 dimensional array used to record 
	// 		whether a cell is on the open list or on the closed list.
	int* openX; //1d array stores the x location of an item on the open list
	int* openY; //1d array stores the y location of an item on the open list
	int** parentX; //2d array to store parent of each cell (x)
	int** parentY; //2d array to store parent of each cell (y)
	int* Fcost;	//1d array to store F cost of a cell on the open list
	int** Gcost; 	//2d array to store G cost for each cell.
	int* Hcost;	//1d array to store H cost of a cell on the open list
	int pathLength;     //stores length of the found path for critter
	int pathLocation;   //stores current position along the chosen path for critter		
	int* pathBank;

	//Path reading variables
	int pathStatus;
	int xPath;
	int yPath;
};

#endif