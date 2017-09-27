//
//  RoadTile.h
//  GAM-1514 Game
//
//  Created by Dan Lingman on 2013-11-09.
//  Copyright (c) 2013 Algonquin College. All rights reserved.
//

#ifndef ROAD_TILE_H
#define ROAD_TILE_H

#include "Tile.h"


class RoadTile : public Tile
{
public:
	RoadTile(const char* textureName = RES_TILE_ROAD);
	virtual ~RoadTile();
  
    //Return the type of the tile
    const char* getType();
	int getWeight();
};

#endif
