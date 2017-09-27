//
//  GrassTile.h
//  GAM-1514 Game
//
//  Created by Dan Lingman on 2013-11-09.
//  Copyright (c) 2013 Algonquin College. All rights reserved.
//

#ifndef GRASS_TILE_H
#define GRASS_TILE_H

#include "Tile.h"


class GrassTile : public Tile
{
public:
	GrassTile(const char* textureName = RES_TILE_GRASS);
	virtual ~GrassTile();
  
    //Return the type of the tile
    const char* getType();
	int getWeight();

};

#endif
