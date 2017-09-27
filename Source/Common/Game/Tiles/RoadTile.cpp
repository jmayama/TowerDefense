//
//  RoadTile.cpp
//  GAM-1514 Game
//
//  Created by Dan Lingman on 2013-11-09.
//  Copyright (c) 2013 Algonquin College. All rights reserved.
//

#include "RoadTile.h"
#include "../../OpenGL/OpenGL.h"
#include "../../Constants/Constants.h"


RoadTile::RoadTile(const char* textureName) : Tile(TileTypeRoad, textureName, true)
{

}

RoadTile::~RoadTile()
{

}

const char* RoadTile::getType()
{
    return TILE_ROAD_TYPE;
}
int RoadTile::getWeight()
{
	return 1;
}