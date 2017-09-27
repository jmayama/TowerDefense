//
//  EnemyUnit.h
//  GAM-1532 OSX Game
//
//  Created by Bradley Flood on 2013-03-07.
//  Copyright (c) 2013 Algonquin College. All rights reserved.
//

#ifndef ENEMY_UNIT_H
#define ENEMY_UNIT_H

#include "GameObject.h"
#include <vector>
#include "Unit.h"
class Level;
class Tile;
class FastPathFinder;

class EnemyUnit : public Unit
{
public:
	EnemyUnit(Level* level);
	virtual ~EnemyUnit();

	//Update, paint and reset methods
	//void update(double delta);
	void paint();
	void paintLife();
	//void reset();

	//Implementing GameObject's pure virtual method
	const char* getType();

	//Setter methods for the current and destination tiles
	void reachedDestination();

protected:
	int m_LifeCounter;
	int m_LifeSpace;
	bool m_DeadEnd;
	OpenGLTexture* m_Life; 
};

#endif