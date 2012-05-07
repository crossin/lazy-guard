/*
;===================================================================
;Modified on A* Pathfinder (Version 1.71a) by Patrick Lester. 
;===================================================================
2012-03-01
*/

#include "PathFinder.h"
#include <stdlib.h>
#include <malloc.h>
#include <math.h>


PathFinder::PathFinder(void)
{
}


PathFinder::~PathFinder(void)
{
	for (int i = 0; i < mapWidth; i++)
	{
		delete []walkability[i];
		delete []whichList[i];
		delete []parentX[i];
		delete []parentY[i];
		delete []Gcost[i];
	}
	delete []walkability;
	delete []whichList;
	delete []parentX;
	delete []parentY;
	delete []Gcost;

	delete []openList;
	delete []openX;
	delete []openY;
	delete []Fcost;
	delete []Hcost;

	delete []pathBank;
	delete []bresenhamBank;
	delete []floydBank;
}

bool PathFinder::initWithSize(int mWeight, int mHeight, int tWeight, int tHeight)
{
	// init map size
	mapWidth = mWeight;
	mapHeight = mHeight;
	tileWidth = tWeight;
	tileHeight = tHeight;

	onClosedList = 10;
	walkability = new char*[mapWidth];
	whichList = new int*[mapWidth];
	parentX = new int*[mapWidth];
	parentY = new int*[mapWidth];
	Gcost = new int*[mapWidth];

	for (int i = 0; i < mapWidth; i++)
	{
		walkability[i] = new char[mapHeight];
		whichList[i] = new int[mapHeight];
		parentX[i] = new int[mapHeight];
		parentY[i] = new int[mapHeight];
		Gcost[i] = new int[mapHeight];
	}
	openList = new int[mapWidth*mapHeight+2];
	openX = new int[mapWidth*mapHeight+2];
	openY = new int[mapWidth*mapHeight+2];
	Fcost = new int[mapWidth*mapHeight+2];
	Hcost = new int[mapWidth*mapHeight+2];

	pathBank = new int[2*(mapWidth*mapHeight)];
	bresenhamBank = new int[2*(mapWidth*mapHeight)];
	floydBank = new int[2*(mapWidth*mapHeight)];
	//InitializePathfinder();
	
	return true;
}

// void PathFinder::InitializePathfinder (void)
// {
// 	pathBank = (int*) malloc(4);
// }


//-----------------------------------------------------------------------------
// Name: EndPathfinder
// Desc: Frees memory used by the pathfinder.
//-----------------------------------------------------------------------------
// void PathFinder::EndPathfinder (void)
// {
// 	free (pathBank);
// }


//-----------------------------------------------------------------------------
// Name: FindPath
// Desc: Finds a path using A*
//-----------------------------------------------------------------------------
int PathFinder::FindPath (int startingX, int startingY,
	int targetX, int targetY)
{
	int onOpenList=0, parentXval=0, parentYval=0,
		a=0, b=0, m=0, u=0, v=0, temp=0, corner=0, numberOfOpenListItems=0,
		addedGCost=0, tempGcost = 0, path = 0,
		tempx, pathX, pathY, cellPosition,
		newOpenListItemID=0;

	//1. Convert location data (in pixels) to coordinates in the walkability array.
	startX = startingX/tileWidth;
	startY = startingY/tileHeight;	
	targetX = targetX/tileWidth;
	targetY = targetY/tileHeight;

	startX = (startX<0) ? 0 : ((startX>=mapWidth) ? (mapWidth-1) : startX);
	startY = (startY<0) ? 0 : ((startY>=mapHeight) ? (mapHeight-1) : startY);
	targetX = (targetX<0) ? 0 : ((targetX>=mapWidth) ? (mapWidth-1) : targetX);
	targetY = (targetY<0) ? 0 : ((targetY>=mapHeight) ? (mapHeight-1) : targetY);
	//2.Quick Path Checks: Under the some circumstances no path needs to
	//	be generated ...

	//	If starting location and target are in the same location...
	if (startX == targetX && startY == targetY && pathLocation > 0)
		return found;
	if (startX == targetX && startY == targetY && pathLocation == 0)
		return same;

	//	If target square is unwalkable, return that it's a nonexistent path.
	if (walkability[targetX][targetY] == unwalkable)
		goto noPath;

	/*
	// If crossable
	if (floydCrossAble(startX, startY, targetX, targetY))
	{
		pathBank[0] = targetX;
		pathBank[1] = targetY;
		pathLength = 1;
		return found;
	}*/

	//3.Reset some variables that need to be cleared
	if (onClosedList > 1000000) //reset whichList occasionally
	{
		for (int x = 0; x < mapWidth;x++) {
			for (int y = 0; y < mapHeight;y++)
				whichList [x][y] = 0;
		}
		onClosedList = 10;	
	}
	onClosedList = onClosedList+2; //changing the values of onOpenList and onClosed list is faster than redimming whichList() array
	onOpenList = onClosedList-1;
	pathLength = notStarted;//i.e, = 0
	pathLocation = notStarted;//i.e, = 0
	Gcost[startX][startY] = 0; //reset starting square's G value to 0

	//4.Add the starting location to the open list of squares to be checked.
	numberOfOpenListItems = 1;
	openList[1] = 1;//assign it as the top (and currently only) item in the open list, which is maintained as a binary heap (explained below)
	openX[1] = startX ; openY[1] = startY;

	//5.Do the following until a path is found or deemed nonexistent.
	do
	{

		//6.If the open list is not empty, take the first cell off of the list.
		//	This is the lowest F cost cell on the open list.
		if (numberOfOpenListItems != 0)
		{

			//7. Pop the first item off the open list.
			parentXval = openX[openList[1]];
			parentYval = openY[openList[1]]; //record cell coordinates of the item
			whichList[parentXval][parentYval] = onClosedList;//add the item to the closed list

			//	Open List = Binary Heap: Delete this item from the open list, which
			//  is maintained as a binary heap. For more information on binary heaps, see:
			//	http://www.policyalmanac.org/games/binaryHeaps.htm
			numberOfOpenListItems = numberOfOpenListItems - 1;//reduce number of open list items by 1	

			//	Delete the top item in binary heap and reorder the heap, with the lowest F cost item rising to the top.
			openList[1] = openList[numberOfOpenListItems+1];//move the last item in the heap up to slot #1
			v = 1;

			//	Repeat the following until the new item in slot #1 sinks to its proper spot in the heap.
			do
			{
				u = v;		
				if (2*u+1 <= numberOfOpenListItems) //if both children exist
				{
					//Check if the F cost of the parent is greater than each child.
					//Select the lowest of the two children.
					if (Fcost[openList[u]] >= Fcost[openList[2*u]]) 
						v = 2*u;
					if (Fcost[openList[v]] >= Fcost[openList[2*u+1]]) 
						v = 2*u+1;		
				}
				else
				{
					if (2*u <= numberOfOpenListItems) //if only child #1 exists
					{
						//Check if the F cost of the parent is greater than child #1	
						if (Fcost[openList[u]] >= Fcost[openList[2*u]]) 
							v = 2*u;
					}
				}

				if (u != v) //if parent's F is > one of its children, swap them
				{
					temp = openList[u];
					openList[u] = openList[v];
					openList[v] = temp;			
				}
				else
					break; //otherwise, exit loop

			}
			while (1);
			//while (!KeyDown(27));//reorder the binary heap


			//7.Check the adjacent squares. (Its "children" -- these path children
			//	are similar, conceptually, to the binary heap children mentioned
			//	above, but don't confuse them. They are different. Path children
			//	are portrayed in Demo 1 with grey pointers pointing toward
			//	their parents.) Add these adjacent child squares to the open list
			//	for later consideration if appropriate (see various if statements
			//	below).
			for (b = parentYval-1; b <= parentYval+1; b++){
				for (a = parentXval-1; a <= parentXval+1; a++){

					//	If not off the map (do this first to avoid array out-of-bounds errors)
					if (a != -1 && b != -1 && a != mapWidth && b != mapHeight){

						//	If not already on the closed list (items on the closed list have
						//	already been considered and can now be ignored).			
						if (whichList[a][b] != onClosedList) { 

							//	If not a wall/obstacle square.
							if (walkability [a][b] != unwalkable) { 

								//	Don't cut across corners
								corner = walkable;	
								if (a == parentXval-1) 
								{
									if (b == parentYval-1)
									{
										if (walkability[parentXval-1][parentYval] == unwalkable
											|| walkability[parentXval][parentYval-1] == unwalkable) 
											corner = unwalkable;
									}
									else if (b == parentYval+1)
									{
										if (walkability[parentXval][parentYval+1] == unwalkable
											|| walkability[parentXval-1][parentYval] == unwalkable) 
											corner = unwalkable; 
									}
								}
								else if (a == parentXval+1)
								{
									if (b == parentYval-1)
									{
										if (walkability[parentXval][parentYval-1] == unwalkable 
											|| walkability[parentXval+1][parentYval] == unwalkable) 
											corner = unwalkable;
									}
									else if (b == parentYval+1)
									{
										if (walkability[parentXval+1][parentYval] == unwalkable 
											|| walkability[parentXval][parentYval+1] == unwalkable)
											corner = unwalkable; 
									}
								}	
								if (corner == walkable) {

									//	If not already on the open list, add it to the open list.			
									if (whichList[a][b] != onOpenList) 
									{	

										//Create a new open list item in the binary heap.
										newOpenListItemID = newOpenListItemID + 1; //each new item has a unique ID #
										m = numberOfOpenListItems+1;
										openList[m] = newOpenListItemID;//place the new open list item (actually, its ID#) at the bottom of the heap
										openX[newOpenListItemID] = a;
										openY[newOpenListItemID] = b;//record the x and y coordinates of the new item

										//Figure out its G cost
										if (abs(a-parentXval) == 1 && abs(b-parentYval) == 1)
											addedGCost = 14;//cost of going to diagonal squares	
										else	
											addedGCost = 10;//cost of going to non-diagonal squares				
										Gcost[a][b] = Gcost[parentXval][parentYval] + addedGCost;

										//Figure out its H and F costs and parent
										Hcost[openList[m]] = 10*(abs(a - targetX) + abs(b - targetY));
										Fcost[openList[m]] = Gcost[a][b] + Hcost[openList[m]];
										parentX[a][b] = parentXval ; parentY[a][b] = parentYval;	

										//Move the new open list item to the proper place in the binary heap.
										//Starting at the bottom, successively compare to parent items,
										//swapping as needed until the item finds its place in the heap
										//or bubbles all the way to the top (if it has the lowest F cost).
										while (m != 1) //While item hasn't bubbled to the top (m=1)	
										{
											//Check if child's F cost is < parent's F cost. If so, swap them.	
											if (Fcost[openList[m]] <= Fcost[openList[m/2]])
											{
												temp = openList[m/2];
												openList[m/2] = openList[m];
												openList[m] = temp;
												m = m/2;
											}
											else
												break;
										}
										numberOfOpenListItems = numberOfOpenListItems+1;//add one to the number of items in the heap

										//Change whichList to show that the new item is on the open list.
										whichList[a][b] = onOpenList;
									}

									//8.If adjacent cell is already on the open list, check to see if this 
									//	path to that cell from the starting location is a better one. 
									//	If so, change the parent of the cell and its G and F costs.	
									else //If whichList(a,b) = onOpenList
									{

										//Figure out the G cost of this possible new path
										if (abs(a-parentXval) == 1 && abs(b-parentYval) == 1)
											addedGCost = 14;//cost of going to diagonal tiles	
										else	
											addedGCost = 10;//cost of going to non-diagonal tiles				
										tempGcost = Gcost[parentXval][parentYval] + addedGCost;

										//If this path is shorter (G cost is lower) then change
										//the parent cell, G cost and F cost. 		
										if (tempGcost < Gcost[a][b]) //if G cost is less,
										{
											parentX[a][b] = parentXval; //change the square's parent
											parentY[a][b] = parentYval;
											Gcost[a][b] = tempGcost;//change the G cost			

											//Because changing the G cost also changes the F cost, if
											//the item is on the open list we need to change the item's
											//recorded F cost and its position on the open list to make
											//sure that we maintain a properly ordered open list.
											for (int x = 1; x <= numberOfOpenListItems; x++) //look for the item in the heap
											{
												if (openX[openList[x]] == a && openY[openList[x]] == b) //item found
												{
													Fcost[openList[x]] = Gcost[a][b] + Hcost[openList[x]];//change the F cost

													//See if changing the F score bubbles the item up from it's current location in the heap
													m = x;
													while (m != 1) //While item hasn't bubbled to the top (m=1)	
													{
														//Check if child is < parent. If so, swap them.	
														if (Fcost[openList[m]] < Fcost[openList[m/2]])
														{
															temp = openList[m/2];
															openList[m/2] = openList[m];
															openList[m] = temp;
															m = m/2;
														}
														else
															break;
													} 
													break; //exit for x = loop
												} //If openX(openList(x)) = a
											} //For x = 1 To numberOfOpenListItems
										}//If tempGcost < Gcost(a,b)

									}//else If whichList(a,b) = onOpenList	
								}//If not cutting a corner
							}//If not a wall/obstacle square.
						}//If not already on the closed list 
					}//If not off the map
				}//for (a = parentXval-1; a <= parentXval+1; a++){
			}//for (b = parentYval-1; b <= parentYval+1; b++){

		}//if (numberOfOpenListItems != 0)

		//9.If open list is empty then there is no path.	
		else
		{
			path = nonexistent; break;
		}  

		//If target is added to open list then path has been found.
		if (whichList[targetX][targetY] == onOpenList)
		{
			path = found; break;
		}

	}
	while (1);//Do until path is found or deemed nonexistent

	//10.Save the path if it exists.
	if (path == found)
	{

		//a.Working backwards from the target to the starting location by checking
		//	each cell's parent, figure out the length of the path.
		pathX = targetX; pathY = targetY;
		do
		{
			//Look up the parent of the current cell.	
			tempx = parentX[pathX][pathY];		
			pathY = parentY[pathX][pathY];
			pathX = tempx;

			//Figure out the path length
			pathLength++;
		}
		while (pathX != startX || pathY != startY);

		//b.Resize the data bank to the right size in bytes
		//pathBank = (int*) realloc (pathBank, pathLength*8);

		//c. Now copy the path information over to the databank. Since we are
		//	working backwards from the target to the start location, we copy
		//	the information to the data bank in reverse order. The result is
		//	a properly ordered set of path data, from the first step to the
		//	last.
		pathX = targetX ; pathY = targetY;
		cellPosition = pathLength*2;//start at the end	
		do
		{
			cellPosition = cellPosition - 2;//work backwards 2 integers
			pathBank[cellPosition] = pathX;
			pathBank[cellPosition+1] = pathY;

			//d.Look up the parent of the current cell.	
			tempx = parentX[pathX][pathY];		
			pathY = parentY[pathX][pathY];
			pathX = tempx;

			//e.If we have reached the starting square, exit the loop.	
		}
		while (pathX != startX || pathY != startY);	

		//11.Read the first path step into xPath/yPath arrays
		ReadPath(startingX,startingY,1);

		// floyd cut
		floyd();
	}
	return path;


	//13.If there is no path to the selected target, set the pathfinder's
	//	xPath and yPath equal to its current location and return that the
	//	path is nonexistent.
noPath:
	xPath = startingX;
	yPath = startingY;
	return nonexistent;
}




//==========================================================
//READ PATH DATA: These functions read the path data and convert
//it to screen pixel coordinates.
void PathFinder::ReadPath(int currentX,int currentY,
	int pixelsPerFrame)
{
	/*
	;	Note on PixelsPerFrame: The need for this parameter probably isn't
	;	that obvious, so a little explanation is in order. This
	;	parameter is used to determine if the pathfinder has gotten close
	;	enough to the center of a given path square to warrant looking up
	;	the next step on the path.
	;	 
	;	This is needed because the speed of certain sprites can
	;	make reaching the exact center of a path square impossible.
	;	In Demo #2, the chaser has a velocity of 3 pixels per frame. Our
	;	tile size is 50 pixels, so the center of a tile will be at location
	;	25, 75, 125, etc. Some of these are not evenly divisible by 3, so
	;	our pathfinder has to know how close is close enough to the center.
	;	It calculates this by seeing if the pathfinder is less than 
	;	pixelsPerFrame # of pixels from the center of the square. 

	;	This could conceivably cause problems if you have a *really* fast
	;	sprite and/or really small tiles, in which case you may need to
	;	adjust the formula a bit. But this should almost never be a problem
	;	for games with standard sized tiles and normal speeds. Our smiley
	;	in Demo #4 moves at a pretty fast clip and it isn't even close
	;	to being a problem.
	*/

	//int ID = pathfinderID; //redundant, but makes the following easier to read

	//If a path has been found for the pathfinder	...
	if (pathStatus == found)
	{

		//If path finder is just starting a new path or has reached the 
		//center of the current path square (and the end of the path
		//hasn't been reached), look up the next path square.
		if (pathLocation < pathLength)
		{
			//if just starting or if close enough to center of square
			if (pathLocation == 0 || (abs(currentX - xPath) < pixelsPerFrame && abs(currentY - yPath) < pixelsPerFrame))
				pathLocation++;
		}

		//Read the path data.		
		xPath = ReadPathX(pathLocation);
		yPath = ReadPathY(pathLocation);

		//If the center of the last path square on the path has been 
		//reached then reset.
		if (pathLocation == pathLength) 
		{
			if (abs(currentX - xPath) < pixelsPerFrame 
				&& abs(currentY - yPath) < pixelsPerFrame) //if close enough to center of square
				pathStatus = notStarted; 
		}
	}

	//If there is no path for this pathfinder, simply stay in the current
	//location.
	else
	{	
		xPath = currentX;
		yPath = currentY;
	}
}


//The following two functions read the raw path data from the pathBank.
//You can call these functions directly and skip the readPath function
//above if you want. Make sure you know what your current pathLocation
//is.

//-----------------------------------------------------------------------------
// Name: ReadPathX
// Desc: Reads the x coordinate of the next path step
//-----------------------------------------------------------------------------
int PathFinder::ReadPathX(int pathLocation)
{
	int x;
	if (pathLocation <= pathLength)
	{

		//Read coordinate from bank
		x = pathBank[pathLocation*2-2];

		//Adjust the coordinates so they align with the center
		//of the path square (optional). This assumes that you are using
		//sprites that are centered -- i.e., with the midHandle command.
		//Otherwise you will want to adjust this.
		x = tileWidth * x + 0.5 * tileWidth;

	}
	return x;
}	


//-----------------------------------------------------------------------------
// Name: ReadPathY
// Desc: Reads the y coordinate of the next path step
//-----------------------------------------------------------------------------
int PathFinder::ReadPathY(int pathLocation)
{
	int y;
	if (pathLocation <= pathLength)
	{

		//Read coordinate from bank
		y = pathBank[pathLocation*2-1];

		//Adjust the coordinates so they align with the center
		//of the path square (optional). This assumes that you are using
		//sprites that are centered -- i.e., with the midHandle command.
		//Otherwise you will want to adjust this.
		y = tileHeight * y + 0.5 * tileHeight;

	}
	return y;
}

void PathFinder::setUnwalkable(int gridX, int gridY, bool isBlock)
{
	walkability [gridX][gridY] = isBlock ? unwalkable : walkable;
}

bool PathFinder::floydCrossAble(int fromX, int fromY, int targetX, int targetY)
{
	bresenhamLength = 0;
	bool steep = abs(targetY - fromY) > abs(targetX - fromX);
	if (steep) {
		int temp = fromX;
		fromX = fromY;
		fromY = temp;
		temp = targetX;
		targetX = targetY;
		targetY = temp;
	}
	int stepX = targetX > fromX?1:(targetX < fromX? -1:0);
	float deltaY = (float)(targetY - fromY) / abs(targetX - fromX);
// 	var ret:Array = [];
	float nowX = fromX + stepX;
	float nowY = fromY + deltaY;
	if (steep)
	{
		if (isUnwalkable(fromY, fromX))
			return false;
	}
	else
	{
		if (isUnwalkable(fromX, fromY))
			return false;
	}
	if (abs(fromX - targetX) == abs(fromY - targetY))
	{
		if (fromX<targetX && fromY<targetY)
		{
			if (isUnwalkable(fromX, fromY+1))
				return false;
			if (isUnwalkable(targetX, targetY-1))
				return false;
		}
		else if (fromX>targetX && fromY>targetY)
		{
			if (isUnwalkable(fromX, fromY-1))
				return false;
			if (isUnwalkable(targetX, targetY+1))
				return false;
		}
		else if (fromX<targetX && fromY>targetY)
		{
			if (isUnwalkable(fromX, fromY-1))
				return false;
			if (isUnwalkable(targetX, targetY+1))
				return false;
		}
		else if (fromX>targetX && fromY<targetY)
		{
			if (isUnwalkable(fromX, fromY+1))
				return false;
			if (isUnwalkable(targetX, targetY-1))
				return false;
		}
	}
	while (nowX != targetX)
	{
		int fY = floor(nowY);
		int cY = ceil(nowY);
		if (steep)
		{
			if (isUnwalkable(fY, nowX))
				return false;
		}
		else
		{
			if (isUnwalkable(nowX, fY))
				return false;
		}
		if (fY != cY) {
			if (steep)
			{
				if (isUnwalkable(cY, nowX))
					return false;
			}
			else
			{
				if (isUnwalkable(nowX, cY))
					return false;
			}
		}
		else if (deltaY != 0)
		{
			if (steep)
			{
				if (isUnwalkable(cY+1, nowX))
					return false;
				if (isUnwalkable(cY-1, nowX))
					return false;
			}
			else
			{
				if (isUnwalkable(nowX, cY+1))
					return false;
				if (isUnwalkable(nowX, cY-1))
					return false;
			}
		}
		nowX += stepX;
		nowY += deltaY;
	}
	if (steep)
	{
		if (isUnwalkable(targetY, targetX))
			return false;
	}
	else
	{
		if (isUnwalkable(targetX, targetY))
			return false;
	}
	return true;
}

inline bool PathFinder::isUnwalkable(int px, int py)
{
	return (px>=0 && px<mapWidth && py>=0 && py<mapHeight && walkability[px][py]==unwalkable);
}

void PathFinder::floyd(void)
{
	if (pathLength <= 1){
		return;
	}

	// check on same line
	int ax, ay, bx, by, cx, cy;
	ax = startX;
	ay = startY;
	bx = pathBank[0];
	by = pathBank[1];
	int floydLength = 1;
	floydBank[0] = startX;
	floydBank[1] = startY;
	for (int i = 0; i <= pathLength-2; i++)
	{
		cx = pathBank[i*2+2];
		cy = pathBank[i*2+3];
		if (!((bx - ax == cx - bx) && (by - ay == cy - by)))
		{
			floydBank[floydLength*2] = pathBank[i*2];
			floydBank[floydLength*2+1] = pathBank[i*2+1];
			floydLength++;
		}
		ax = bx;
		ay = by;
		bx = cx;
		by = cy;
	}
	floydBank[floydLength*2] = pathBank[pathLength*2-2];
	floydBank[floydLength*2+1] = pathBank[pathLength*2-1];
	floydLength++;

// pathLength=floydLength;
// for (int i = 0; i <pathLength*2; i++)
// {	
// 	pathBank[i] = floydBank[i];
// }

	pathLength = 0;
	for (int i = 0; i < floydLength-1; i++)
	{
		if (i == floydLength-2)
		{
			pathBank[pathLength*2] = floydBank[2*i+2];
			pathBank[pathLength*2+1] = floydBank[2*i+3];
			pathLength++;
		}
		for (int j = floydLength-1; j > i+1; j--)
		{
			if (floydCrossAble(floydBank[2*i], floydBank[2*i+1], floydBank[2*j], floydBank[2*j+1]))
			{
				pathBank[pathLength*2] = floydBank[2*j];
				pathBank[pathLength*2+1] = floydBank[2*j+1];
				pathLength++;
				i = j - 1;
				break;
			}
			if (j == i+2)
			{
				pathBank[pathLength*2] = floydBank[2*i+2];
				pathBank[pathLength*2+1] = floydBank[2*i+3];
				pathLength++;
			}
		}

	}
}


