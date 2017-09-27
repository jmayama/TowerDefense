//
//  TreeTile.cpp
//  GAM-1514 Game
//
//  Created by Dan Lingman on 2013-11-09.
//  Copyright (c) 2013 Algonquin College. All rights reserved.
//

#include "TreeTile.h"
#include "../../OpenGL/OpenGL.h"
#include "../../Constants/Constants.h"


TreeTile::TreeTile(const char* textureName) : Tile(TileTypeTree, textureName, true)
{

}

TreeTile::~TreeTile()
{

}

const char* TreeTile::getType()
{
    return TILE_TREE_TYPE;
}
int TreeTile::getWeight()
{
	return 10;
}
