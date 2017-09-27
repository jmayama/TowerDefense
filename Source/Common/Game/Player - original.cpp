//
//  Player.cpp
//  GAM-1532 OSX Game
//
//  Created by Bradley Flood on 2013-03-07.
//  Copyright (c) 2013 Algonquin College. All rights reserved.
//

#include "Player.h"
#include "Level.h"
#include "Tiles/Tile.h"
#include "../OpenGL/OpenGL.h"
#include "../Constants/Constants.h"
#include <stdlib.h>
#include <algorithm>
#include <math.h>
#include "../Path/FastPathFinder.h"
#include "../Utils/Logger/Logger.h"

Player::Player(Level* aLevel)
{
    //Initialize the current and destination tiles to NULL
    m_CurrentTile = NULL;
    m_DestinationTile = NULL;
    
    //Initialize the animation member variables
    m_CanAnimate = false;
    m_AbortAnimation = false;
    m_AnimationPathNodeIndex = -1;
    
    //Initialize the player's size
    setSize((float)PLAYER_SIZE, (float)PLAYER_SIZE);
	m_Level = aLevel;
	m_FastPathFinder = NULL;
	m_StartRequested = false;
	m_State = AT_DESTINATION;
}

Player::~Player()
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

FastPathFinder *Player::getPathFinder()
{
	return m_FastPathFinder;
}

void Player::getCoordinatesFromTile(Tile* tile,int &x,int &y)
{
	float tx = tile->getX();
	float ty = tile->getY();
	x = m_Level->getTileCoordinateForPosition(tx);
	y = m_Level->getTileCoordinateForPosition(ty);
}

void Player::update(double aDelta)
{
	switch(m_State)
	{
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
			m_Duration = EMPTY_LEVEL_TILE_SIZE/PLAYER_SPEED*m_NextTile->getWeight();
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
			setX(m_SX + m_ElapsedTime * (m_TX-m_SX)/32*PLAYER_SPEED/m_NextTile->getWeight());
			setY(m_SY + m_ElapsedTime * (m_TY-m_SY)/32*PLAYER_SPEED/m_NextTile->getWeight());
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
				m_Duration = EMPTY_LEVEL_TILE_SIZE/PLAYER_SPEED*m_NextTile->getWeight();
				m_State = MOVING;
			}
		}
		break;
	case AT_DESTINATION:
		if (m_StartRequested)
		{
			m_State = START;
		}
		break;
	case OBSTACLE:
		// will get back to this.
		break;
	}
}

void Player::paint()
{
	OpenGLRenderer::getInstance()->setForegroundColor(PLAYER_INSIDE_COLOR);
	OpenGLRenderer::getInstance()->drawCircle(getX() + (getWidth() / 2), getY() + (getHeight() / 2), getWidth() / 2,true,12);
	OpenGLRenderer::getInstance()->setForegroundColor(PLAYER_OUTLINE_COLOR);
	OpenGLRenderer::getInstance()->drawCircle(getX() + (getWidth() / 2), getY() + (getHeight() / 2), getWidth() / 2, false,12);
}

void Player::reset()
{
    m_DestinationTile = NULL;
	m_FastPathFinder = new FastPathFinder(m_Level);
}

const char* Player::getType()
{
    return PLAYER_TYPE;
}

void Player::setCurrentTile(Tile* tile)
{
	//Set the current tile pointer
	m_CurrentTile = tile;
    
	//Center the player's position on the tile
	setPosition(tile->getX() + ((tile->getWidth() - getWidth()) / 2), tile->getY() + ((tile->getHeight() - getHeight()) / 2));
}

void Player::setDestinationTile(Tile* tile)
{
	//Set the destination tile pointer
	m_DestinationTile = tile;
	m_StartRequested = true;
}

float Player::animate(float aCurrent, float aTarget, double aDelta)
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

void Player::startAnimating()
{
	m_CanAnimate = true;
	m_AnimationPathNodeIndex = 0;
}

void Player::stopAnimating()
{
	m_CanAnimate = false;
	m_AnimationPathNodeIndex = -1;
}

bool Player::isAnimating()
{
    return m_CanAnimate;
}
