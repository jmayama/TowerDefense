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

Player::Player(Level* aLevel) :Unit(aLevel)
{
    setScore(20);
}

Player::~Player()
{
    
}

const char* Player::getType()
{
    return PLAYER_TYPE;
}

//Where the player is currently..
Tile *Player::getCurrentTile()
{
	return m_CurrentTile;
}


