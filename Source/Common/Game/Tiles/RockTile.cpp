//
//  RockTile.cpp
//  GAM-1514 Game
//
//  Created by Dan Lingman on 2013-11-09.
//  Copyright (c) 2013 Algonquin College. All rights reserved.
//

#include "RockTile.h"
#include "../../OpenGL/OpenGL.h"
#include "../../Constants/Constants.h"


RockTile::RockTile(const char* textureName) : Tile(TileTypeRock, textureName, true)
{

}

RockTile::~RockTile()
{

}

const char* RockTile::getType()
{
    return TILE_ROCK_TYPE;
}
int RockTile::getWeight()
{
	return 100;
}

bool RockTile::isWalkableTile()
{
	return false;
}