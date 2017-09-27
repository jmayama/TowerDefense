//
//  SpawnPoint.cpp
//  GAM-1532 OSX Game
//
//  Created by Bradley Flood on 2013-03-07.
//  Copyright (c) 2013 Algonquin College. All rights reserved.
//

#include "SpawnPoint.h"
#include "Level.h"
#include "Tiles/Tile.h"
#include "../OpenGL/OpenGL.h"
#include "../Constants/Constants.h"
#include <stdlib.h>
#include <algorithm>
#include <math.h>
#include "../Path/FastPathFinder.h"
#include "../Utils/Logger/Logger.h"

SpawnPoint::SpawnPoint(Level* aLevel)
{
	m_Level = aLevel;
	m_ElapsedTime = 0.0;
	m_SpawnRate = 2.0;
}

SpawnPoint::~SpawnPoint()
{
    
}

const char* SpawnPoint::getType()
{
    return SPAWN_POINT_TYPE; //PLAYER_TYPE; 
}
void SpawnPoint::paint()
{
	OpenGLRenderer::getInstance()->setForegroundColor(OpenGLColorRed());
	OpenGLRenderer::getInstance()->drawRectangle(getX(), getY(), 32.0f, 32.0f,true);

	
}

void SpawnPoint::update(double aDelta)
{
	m_ElapsedTime += aDelta;
	if (m_ElapsedTime >= m_SpawnRate)
	{
		m_ElapsedTime = 0.0;
		// spawn a new unit
		EnemyUnit * unit = new EnemyUnit(m_Level);
		unit->setCurrentTile(m_CurrentTile);
		unit->setScore(15);
		unit->reset();
		m_Level->addEnemyUnit(unit);

	}
}
//Putting the spawn point to 0,0 here
void SpawnPoint::setCurrentTile(Tile* tile)
{
	m_CurrentTile = tile;
	setPosition(tile->getX(), tile->getY()); //setPosition(tile->getX(), tile->getY()); X = 800 Y = 736
}




