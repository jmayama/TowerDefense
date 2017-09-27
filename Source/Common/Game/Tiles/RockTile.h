//
//  RockTile.h
//  GAM-1514 Game
//
//  Created by Dan Lingman on 2013-11-09.
//  Copyright (c) 2013 Algonquin College. All rights reserved.
//

#ifndef ROCK_TILE_H
#define ROCK_TILE_H

#include "Tile.h"


class RockTile : public Tile
{
public:
	RockTile(const char* textureName = RES_TILE_ROCK);
	virtual ~RockTile();
	virtual bool isWalkableTile();
    //Return the type of the tile
    const char* getType();
	int getWeight();
};

#endif
