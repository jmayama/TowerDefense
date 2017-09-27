//
//  GrassTile.cpp
//  GAM-1514 Game
//
//  Created by Dan Lingman on 2013-11-09.
//  Copyright (c) 2013 Algonquin College. All rights reserved.
//

#include "GrassTile.h"
#include "../../OpenGL/OpenGL.h"
#include "../../Constants/Constants.h"


GrassTile::GrassTile(const char* textureName) : Tile(TileTypeGrass, textureName, true)
{

}

GrassTile::~GrassTile()
{

}

const char* GrassTile::getType()
{
    return TILE_GRASS_TYPE;
}
int GrassTile::getWeight()
{
	return 4;
}
