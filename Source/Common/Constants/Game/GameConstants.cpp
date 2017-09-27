//
//  GameConstants.cpp
//  GAM-1514 Game
//
//  Modified by Dan Lingman on 2013-11-09.
//  Copyright (c) 2013 Algonquin College. All rights reserved.
// added tile types
//

#include "GameConstants.h"


//Game Constants
const char* GAME_SCREEN_NAME = "Game";

//Tiles Constants
const char* TILE_GROUND_TYPE = "GroundTile";
const char* TILE_GRASS_TYPE = "GrassTile";
const char* TILE_ROAD_TYPE = "RoadTile";
const char* TILE_ROCK_TYPE = "RockTile";
const char* TILE_TREE_TYPE = "TreeTile";
const char* TILE_WATER_TYPE = "WaterTile";
const int TILE_PADDING = 2;

//Player Constants
const char* PLAYER_TYPE = "Player";
const char* ENEMY_TYPE = "Enemy";
const char* SPAWN_POINT_TYPE = "Enemy";
const int PLAYER_SIZE = 24;
const float PLAYER_SPEED = 150.0f; // 150.0f originally
const OpenGLColor PLAYER_INSIDE_COLOR = OpenGLColor(0.0f, 0.0f, 0.0f, 1.0f);
const OpenGLColor PLAYER_OUTLINE_COLOR = OpenGLColor(1.0f, 1.0f, 1.0f, 1.0f);

//Life Constants
const char* GAME_LIFE_TEXTURE = "LifeTexture";

//Game Constants
const double GAME_OVER_TIMER = 2.5;
