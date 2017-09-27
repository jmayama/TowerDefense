//
//  PathFinder.cpp
//  GAM-1532 OSX Game
//
//  Created by Bradley Flood on 2013-03-07.
//  Copyright (c) 2013 Algonquin College. All rights reserved.
//

#include "PathFinder.h"
#include "PathNode.h"
#include "../Game/Level.h"
#include "../Game/Tiles/Tile.h"
#include "../Constants/Constants.h"
#include <stdlib.h>
#include <algorithm>
#include <math.h>



PathFinder::PathFinder(Level* level, PathFinderListener* listener) :
    m_Level(level),
    m_Listener(listener),
    m_State(StateIdle),
    m_DestinationTileIndex(-1),
    m_SearchDelay(0.0),
    m_EnableSearchDelay(false)
{

}

PathFinder::~PathFinder()
{
    clearPathNodes();
}

void PathFinder::findPath(Tile* aCurrentTile, Tile* aDestinationTile)
{
    //Safety check that the pathfinder isn't already in the process of finding a different path
    if(m_State != StateIdle)
    {
        return;
    }

    //Cycle through and reset all the tiles path flags to false (for ground tiles only)
    for(int i = 0; i < m_Level->getNumberOfTiles(); i++)
    {
        Tile* tile = m_Level->getTileForIndex(i);
        if(tile != NULL && tile->isWalkableTile() == true)
        {
            tile->setIsPath(false);
        }
    }
    
    //Clear the path nodes from any previous path finding
	clearPathNodes();
    
	//Get the current tile and destination tile indexes
	int currentTileIndex = m_Level->getTileIndexForTile(aCurrentTile);
	m_DestinationTileIndex = m_Level->getTileIndexForTile(aDestinationTile);
    
	//Make sure the destination and the current tile aren't the same tile
	if(currentTileIndex == m_DestinationTileIndex)
	{
		return;
	}
    
	//Make sure the destination is not a wall tile
	if(m_Level->getTileForIndex(m_DestinationTileIndex)->isWalkableTile() == false)
	{
		return;
    }
    
    //Allocate the current tile's path node and add it to the open vector
	PathNode* pathNode = new PathNode(aCurrentTile);
	addPathNodeToOpenList(pathNode);
    
    //Set the path finder's state to searching
    m_State = StateSearchingPath;
    
    //Set the search delay to zero
    m_SearchDelay = 0.0;
}

void PathFinder::update(double aDelta)
{
    if(m_SearchDelay > 0.0)
    {
        m_SearchDelay -= aDelta;
        if(m_SearchDelay <= 0.0)
        {
            m_SearchDelay = 0.0;
        }
        return;
    }
    
    if(isSearchingPath() == true && m_DestinationTileIndex != -1)
    {
        //If the open list has no path nodes in it, then the destination tile
        //can't be reached and we have an error
        if(m_PathNodeOpen.size() == 0)
        {
            //Set the state to error
            m_State = StateError;
            
            //Notify the listener
            if(m_Listener != NULL)
            {
                m_Listener->pathFinderFinishedSearching(this, false);
            }
            
            return;
        }
        
		//Because the list is ordered, the first step is always the one with the lowest F cost
		PathNode* currentNode = m_PathNodeOpen.front();
        
		//Add the current node to the closed vector
		m_PathNodeClosed.push_back(currentNode);
        
		//Remove it from the open list
		//Note that if we wanted to first removing from the open list, care should be taken to the memory
		m_PathNodeOpen.erase(m_PathNodeOpen.begin());
        
        //Get the current node tile index
        int currentNodeTileIndex = m_Level->getTileIndexForTile(currentNode->getTile());
        
		//Does the current tile index equal the destination tile index?
		if(currentNodeTileIndex == m_DestinationTileIndex)
		{
            //Build the final node path
			buildFinalNodePath(currentNode);
            
            //Set the state to path found
            m_State = StateFoundPath;
            
            //Notify the listener
            if(m_Listener != NULL)
            {
                m_Listener->pathFinderFinishedSearching(this, true);
            }
            
			return;
		}
        
		//Use a vector to hold all the adjacent tiles to the current path node
		std::vector<Tile*> adjacentTiles;
        
		//Top
        addAdjacentTile(adjacentTiles, currentNode->getTile(), 0, -1);
 		//Bottom
        addAdjacentTile(adjacentTiles, currentNode->getTile(), 0, 1);
        
		//Left
        addAdjacentTile(adjacentTiles, currentNode->getTile(), -1, 0);
        
       
		//Right
        addAdjacentTile(adjacentTiles, currentNode->getTile(), 1, 0);
        
		//Cycle through all the adjacent tiles
		for(int i = 0; i < adjacentTiles.size(); i++)
		{
            //Get the adjacent tile from the vector at index i
			Tile* adjacentTile = adjacentTiles.at(i);
			
			//Does the tile exist in the closed list? If it does skip it
            if(doesTileExistInClosedList(adjacentTile) == true)
            {
				continue;
			}
            
			//Does the tile exist in the open list?
			if(doesTileExistInOpenList(adjacentTile) == false)
			{
                //If it doesn't, create a new PathNode for the adjacent tile
                PathNode* adjacentNode = new PathNode(adjacentTile);
                
				//Set the parent node of the adjacent tile
				adjacentNode->setParentNode(currentNode);
                
                //Calculate the G and H scores
				adjacentNode->setScoreG(currentNode->getScoreG() + 1);
				adjacentNode->setScoreH(getManhattanDistanceCost(currentNode->getTile(), m_Level->getTileForIndex(m_DestinationTileIndex)));
//				adjacentNode->setScoreH(getManhattanDistanceCost(adjacentNode->getTile(), m_Level->getTileForIndex(m_DestinationTileIndex)));
                
				//Adding it with the function which is preserving the list ordered by F score
				addPathNodeToOpenList(adjacentNode);
			}
			else
			{
                //Get the existing node in the Open List for the adjacent tile
                PathNode* existingNode = getOpenPathNodeForTile(adjacentTile);
                
				//Check to see if the G score for that node is lower, if we use the current step to get there
                //Since the H score will be the exact same for both nodes, we only need to compare the G scores
				if((currentNode->getScoreG() + 1) < existingNode->getScoreG())
				{
					//The G score is equal to the parent G score + the cost to move from the parent to it
					existingNode->setScoreG(currentNode->getScoreG() + 1);
                    
                    //Set the new parent node
                    existingNode->setParentNode(currentNode);
					
                    //Sort the open list
                    sortOpenList();
				}
			}
		}
        
		//Clear the Tiles out of the adjacent tiles vector
		adjacentTiles.clear();
    }
    
    //If the search delay is enabled, set the delay timer
    if(m_EnableSearchDelay == true)
    {
        m_SearchDelay = PATH_FINDING_DELAY;
    }
}

void PathFinder::paint()
{
    //Paint the open list path scoring
    for(int i = 0; i < m_PathNodeOpen.size(); i++)
    {
        m_PathNodeOpen.at(i)->paintScore(OpenGLColorRed());
    }
    
    //Paint the closed list path scoring
    for(int i = 0; i < m_PathNodeClosed.size(); i++)
    {
        m_PathNodeClosed.at(i)->paintScore(OpenGLColorBlue());
    }
    
    //Paint the final path scoring
    for(int i = 0; i < m_PathNodeFinal.size(); i++)
    {
        m_PathNodeFinal.at(i)->paintScore(OpenGLColorYellow());
    }
}

void PathFinder::reset()
{
    m_State = StateIdle;
}

void PathFinder::addAdjacentTile(std::vector<Tile*>& aAdjacentTiles, Tile* aCurrentTile, int aDeltaX, int aDeltaY)
{
    //Get the adjacent tile coordinates
    int adjacentTileCoordinateX = m_Level->getTileCoordinateForPosition(aCurrentTile->getX()) + aDeltaX;
    int adjacentTileCoordinateY = m_Level->getTileCoordinateForPosition(aCurrentTile->getY()) + aDeltaY;
    
    //Get the adjacent tile pointer from the level for the coordinates
    Tile* adjacentTile = m_Level->getTileForCoordinates(adjacentTileCoordinateX, adjacentTileCoordinateY);
    if(adjacentTile != NULL)
    {
        //Is the tile walkable and valid (ie not outside the bounds of the level)
        bool isAdjacentTileWalkable = adjacentTile->isWalkableTile();
        bool isAdjacentTileValid = m_Level->validateTileCoordinates(adjacentTileCoordinateX, adjacentTileCoordinateY);
        
        //If both are true, then add the adjacent tile to the vector that was passed by reference
        if(isAdjacentTileValid == true && isAdjacentTileWalkable == true)
        {
            aAdjacentTiles.push_back(adjacentTile);
        }
    }
}

bool PathFinder::doesTileExistInClosedList(Tile* aTile)
{
    //Get the tile index from the level for the tile pointer
    int tileIndex = m_Level->getTileIndexForTile(aTile);
    
    //Cycle through the closed list and compare the tile indexes
    for(int i = 0; i < m_PathNodeClosed.size(); i++)
    {
        PathNode* pathNode = m_PathNodeClosed.at(i);
        if(m_Level->getTileIndexForTile(pathNode->getTile()) == tileIndex)
        {
            return true;
        }
    }
    
    //The tile doesn't exist in the closed list
    return false;
}

bool PathFinder::doesTileExistInOpenList(Tile* aTile)
{
    return getOpenPathNodeForTile(aTile) != NULL;
}

PathNode* PathFinder::getOpenPathNodeForTile(Tile* aTile)
{
    //Get the tile index from the level for the tile pointer
    int tileIndex = m_Level->getTileIndexForTile(aTile);
    
    //Cycle through the open list and compare the tile indexes
    for(int i = 0; i < m_PathNodeOpen.size(); i++)
    {
        PathNode* pathNode = m_PathNodeOpen.at(i);
        if(m_Level->getTileIndexForTile(pathNode->getTile()) == tileIndex)
        {
            return pathNode;
        }
    }
    
    //The tile doesn't exist in the open list, return NULL
    return NULL;
}

bool PathFinder::isSearchingPath()
{
    return m_State == StateSearchingPath;
}

int PathFinder::getPathSize()
{
    return m_PathNodeFinal.size();
}

void PathFinder::sortOpenList()
{
    std::sort(m_PathNodeOpen.begin(), m_PathNodeOpen.end(), PathNode::compareNodes);
}

PathNode* PathFinder::getPathNodeAtIndex(int aIndex)
{
    if(aIndex >= 0 && aIndex < getPathSize())
    {
        return m_PathNodeFinal.at(aIndex);
    }
    return NULL;
}

void PathFinder::addPathNodeToOpenList(PathNode* aPathNode)
{
	//Insert the Path node into the Open path node vector
	m_PathNodeOpen.push_back(aPathNode);
    
    //Sort the open list
    sortOpenList();
}

void PathFinder::buildFinalNodePath(PathNode* aPathNode)
{
	do
	{
        //Safety check the parentNode
		if(aPathNode->getParentNode() != NULL)
		{
			m_PathNodeFinal.insert(m_PathNodeFinal.begin(), aPathNode);
		}
        
		//Set the tile path flag to true
		aPathNode->getTile()->setIsPath(true);
        
		//Set the path node's pointer to it's parent
		aPathNode = aPathNode->getParentNode();
	}
	while (aPathNode != NULL);
}

void PathFinder::clearPathNodes()
{
	//Now cycle through the Open node path vector, and delete all the path node
	while(m_PathNodeOpen.size() > 0)
	{
		//Get the last element in the vector
		PathNode* node = m_PathNodeOpen.back();
        
		//Delete the path node
		delete node;
        
		//Remove the last element in the vector
		m_PathNodeOpen.pop_back();
	}
    
	//Lastly cycle through the Closed node path vector, and delete all the path node
	while(m_PathNodeClosed.size() > 0)
	{
		//Get the last element in the vector
		PathNode* node = m_PathNodeClosed.back();
        
		//Delete the path node
		delete node;
        
		//Remove the last element in the vector
		m_PathNodeClosed.pop_back();
	}
    
    //Clear the final path node list
    m_PathNodeFinal.clear();
    
    //Reset the destination tile index
    m_DestinationTileIndex = -1;
}

void PathFinder::togglePathFindingDelay()
{
    m_EnableSearchDelay = !m_EnableSearchDelay;
}

int PathFinder::getManhattanDistanceCost(Tile* aStartTile, Tile* aDestinationTile)
{
	//Here we use the Manhattan method, which calculates the total number of step moved horizontally and vertically to reach the
	//final desired step from the current step, ignoring any obstacles that may be in the way
	int distance = abs(m_Level->getTileCoordinateForPosition(aDestinationTile->getX()) - m_Level->getTileCoordinateForPosition(aStartTile->getX())) 
    + abs(m_Level->getTileCoordinateForPosition(aDestinationTile->getY()) - m_Level->getTileCoordinateForPosition(aStartTile->getY()));
    
    //Return the distance between the two tiles
	return distance;
}