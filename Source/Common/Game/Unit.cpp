//
//  Unit.cpp
//  GAM-1532 OSX Game
//
//  Created by Bradley Flood on 2013-03-07.
//  Copyright (c) 2013 Algonquin College. All rights reserved.
//

#include "Unit.h"
#include "Level.h"
#include "Tiles/Tile.h"
#include "../OpenGL/OpenGL.h"
#include "../Constants/Constants.h"
#include <stdlib.h>
#include <algorithm>
#include <math.h>
#include "../Path/FastPathFinder.h"
#include "../Utils/Logger/Logger.h"

Unit::Unit(Level* aLevel)
{
	//Initialize the current and destination tiles to NULL
	m_CurrentTile = NULL;
	m_DestinationTile = NULL;

	//Initialize the animation member variables
	m_CanAnimate = false;
	m_AbortAnimation = false;
	m_AnimationPathNodeIndex = -1;
	m_DeleteRequested = false;
	m_ResetRequested = false;
	//Initialize the player's size
	setSize((float)PLAYER_SIZE, (float)PLAYER_SIZE);
	m_Level = aLevel;
	m_FastPathFinder = NULL;
	m_StartRequested = false;
	m_State = AT_DESTINATION;
}

Unit::~Unit()
{
	//Set the current and desination tiles to NULL
	m_CurrentTile = NULL;
	m_DestinationTile = NULL;
	if (m_FastPathFinder != NULL)
	{
		delete m_FastPathFinder;
		m_FastPathFinder = NULL;
	}
}

FastPathFinder *Unit::getPathFinder()
{
	return m_FastPathFinder;
}

void Unit::deleteRequested()
{
	//delete 
}
void Unit::getCoordinatesFromTile(Tile* tile,int &x,int &y)
{
	float tx = tile->getX();
	float ty = tile->getY();
	x = m_Level->getTileCoordinateForPosition((int)tx);
	y = m_Level->getTileCoordinateForPosition((int)ty);
}

void Unit::update(double aDelta)
{
	if (m_DeleteRequested)
	{
		delete this;
		return; // Self termination - someone else asked us to go away.
		// doing it like this means that we won't be in the middle of processing stuff
		// when we evaporate.
	}
	if (m_ResetRequested == false) {
		switch(m_State)
		{
		case STATIONARY:
			break;
		case START:
			int sx,sy,tx,ty;
			getCoordinatesFromTile(m_CurrentTile,sx,sy);
			getCoordinatesFromTile(m_DestinationTile,tx,ty);
			m_FastPathFinder->getPathBetweenCoordinates(sx,sy,tx,ty);
			m_PathIndex = -1;
			m_StartRequested = false;
			if (m_FastPathFinder->getPathLength() == 1)
			{
				m_State = AT_DESTINATION;
			}
			else {
				m_SX = getX();
				m_SY = getY();
				Node * nextNode = m_FastPathFinder->getPathElement(0);
				m_NextTile = m_Level->getTileForCoordinates(nextNode->getX(),nextNode->getY());
				m_TX = m_NextTile->getX()+3;
				m_TY = m_NextTile->getY()+3;
				m_Duration = EMPTY_LEVEL_TILE_SIZE/PLAYER_SPEED;
				m_State = ABOUT_TO_MOVE;
			}
			break;
		case ABOUT_TO_MOVE: // this is here to avoid stuttering at the start - if the path finding took too long, then the next call would make it jump
			m_State = MOVING;
			m_ElapsedTime = 0;
			break;
		case MOVING:
			m_ElapsedTime += aDelta;
			if (m_ElapsedTime >= m_Duration)
			{
				m_State = AT_NEXT_NODE;
				setX(m_TX);
				setY(m_TY);
			}
			else
			{
				setX((float)(m_SX + m_ElapsedTime * (m_TX-m_SX)/32*PLAYER_SPEED));
				setY((float)(m_SY + m_ElapsedTime * (m_TY-m_SY)/32*PLAYER_SPEED));
			}
			break;
		case AT_NEXT_NODE:
			if (m_StartRequested)
			{
				m_State = START;
			}
			else {
				m_PathIndex++;
				if (m_PathIndex == m_FastPathFinder->getPathLength()-1)
				{
					m_State = AT_DESTINATION;
					// didn't update the current tile though.
					Node * currentNode = m_FastPathFinder->getPathElement(m_PathIndex);
					m_CurrentTile = m_Level->getTileForCoordinates(currentNode->getX(),currentNode->getY());
				}
				else {
					Node * currentNode = m_FastPathFinder->getPathElement(m_PathIndex);
					Node * nextNode = m_FastPathFinder->getPathElement(m_PathIndex+1);
					m_CurrentTile = m_Level->getTileForCoordinates(currentNode->getX(),currentNode->getY());
					m_NextTile = m_Level->getTileForCoordinates(nextNode->getX(),nextNode->getY());
					m_SX = m_CurrentTile->getX()+3;
					m_SY = m_CurrentTile->getY()+3;
					m_TX = m_NextTile->getX()+3;
					m_TY = m_NextTile->getY()+3;
					m_ElapsedTime = 0;
					m_Duration = EMPTY_LEVEL_TILE_SIZE/PLAYER_SPEED;
					m_State = MOVING;
				}
			}
			break;
		case AT_DESTINATION:
			if (m_StartRequested)
			{
				m_State = START;
			}
			reachedDestination();
			break;
		case OBSTACLE:
			// will get back to this.
			break;
		}
	}
	else
	{
		if (m_FastPathFinder != NULL)
		{
			m_FastPathFinder->reset();
		}
		else
		{
			m_FastPathFinder = new FastPathFinder(m_Level);
		}
		m_ResetRequested = false;
		m_State = AT_DESTINATION;
	}
}
void Unit::reachedDestination()
{

}

void Unit::paint()
{
	OpenGLRenderer::getInstance()->setForegroundColor(PLAYER_INSIDE_COLOR);
	OpenGLRenderer::getInstance()->drawCircle(getX() + (getWidth() / 2), getY() + (getHeight() / 2), getWidth() / 2,true,12);
	OpenGLRenderer::getInstance()->setForegroundColor(PLAYER_OUTLINE_COLOR);
	OpenGLRenderer::getInstance()->drawCircle(getX() + (getWidth() / 2), getY() + (getHeight() / 2), getWidth() / 2, false,12);
	
}

void Unit::reset()
{
	m_ResetRequested = true; // defer until safe to do

}

Tile* Unit::getDestinationTile()
{
	
	return m_DestinationTile;
}

//set the spawn point...?
void Unit::setCurrentTile(Tile* tile)
{
	//Set the current tile pointer
	m_CurrentTile = tile;

	//Center the player's position on the tile
	setPosition(tile->getX() + ((tile->getWidth() - getWidth()) / 2), tile->getY() + ((tile->getHeight() - getHeight()) / 2));
}

//destination is the place where the player click...the enemy unit will go there 
void Unit::setDestinationTile(Tile* tile)
{
	//Set the destination tile pointer
	//setPosition(tile->getX() + ((tile->getWidth() - getWidth()) / 2), tile->getY() + ((tile->getHeight() - getHeight()) / 2));
	m_DestinationTile = tile;
	//setPosition(tile->getX(), tile->getY()); //800 736. SetPosition for the destinationTile
	m_StartRequested = true;
	m_Level->queueForPathFinding(this);
}

float Unit::animate(float aCurrent, float aTarget, double aDelta)
{
	float increment = (float)(aDelta * PLAYER_SPEED * (aTarget < aCurrent ? -1 : 1));
	if(fabs(increment) > fabs(aTarget - aCurrent))
	{
		return aTarget;
	}
	else
	{
		aCurrent += increment;
	}
	return aCurrent;
}

void Unit::startAnimating()
{
	m_CanAnimate = true;
	m_AnimationPathNodeIndex = 0;
}

void Unit::stopAnimating()
{
	m_CanAnimate = false;
	m_AnimationPathNodeIndex = -1;
}

bool Unit::isAnimating()
{
	return m_CanAnimate;
}

float Unit::getScore()
{
	return m_Score;
}

void Unit::setScore(float score)
{
	m_Score = score;
}

void Unit::setState(PathState state)
{
	m_State = state;
}
