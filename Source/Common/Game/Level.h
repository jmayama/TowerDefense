//
//  Level.h
//  GAM-1532 OSX Game
//
//  Created by Bradley Flood on 2013-03-07.
//  Copyright (c) 2013 Algonquin College. All rights reserved.
// Modified by Dan Lingman on 2013-11-09
// added clear method
//

#ifndef LEVEL_H
#define LEVEL_H

#include "../Constants/Constants.h"
#include "../Path/FastPathFinder.h"
#include "../OpenGL/OpenGL.h"
#include <queue>
#include <vector>
#include "SpawnPoint.h"
#include "Unit.h"
#include "EnemyUnit.h"

class mycomparison {
public: 
	mycomparison() 
	{}
	bool operator() (Unit*& lhs, Unit*&rhs) const 
	{ 
		// assume Unit has a getScore method that tells us 
		// how important this Unit is.
		return ((lhs->getScore()) < (rhs->getScore())); 
	} 
};

typedef std::priority_queue<Unit *, std::vector<Unit *>,mycomparison> mypq_type;


class Tile;
class Player;

class Level
{
public:
	Level(bool isEditingLevel = false);
	~Level();

	//Update, paint and reset methods
	void update(double delta);
	void paint();
	void reset();

	//GAME OVER
	void gameOver();

	//
	void load(const char* levelName);
	void save(const char* levelName);


	//Input methods
	void mouseLeftClickUpEvent(float positionX, float positionY);
	void keyUpEvent(int keyCode);
	void clear();
	//
	TileType getTileTypeForIndex(int index);

	//Tile count methods
	unsigned int getNumberOfTiles();
	unsigned int getNumberOfHorizontalTiles();
	unsigned int getNumberOfVerticalTiles();

	//Validate that the tile coordinates passed in are valid
	bool validateTileCoordinates(int coordinatesX, int coordinatesY);

	//Converts a position in screen space into a position in coordinate space
	int getTileCoordinateForPosition(int position);


	//Index methods
	int getTileIndexForPosition(int positionX, int positionY);
	int getTileIndexForCoordinates(int coordinatesX, int coordinatesY);
	int getTileIndexForTile(Tile* tile);

	//Tile methods
	Tile* getTileForPosition(int positionX, int positionY);
	Tile* getTileForCoordinates(int coordinatesX, int coordinatesY);
	Tile* getTileForIndex(int index);

	//
	void setTileTypeAtPosition(TileType tileType, int positionX, int positionY);
	void setTileTypeAtCoordinates(TileType tileType, int coordinatesX, int coordinatesY);
	void setTileTypeAtIndex(TileType tileType, int index);

	//Coveniance methods to toggle debug paint methods
	void togglePaintTileScoring();
	void togglePaintTileIndexes();
	void queueForPathFinding (Unit * unit);   
	void addEnemyUnit(Unit *unit);
	Tile* getPlayerTile();

	//Final destination tile for the enemies
	Tile* getFinalDestinationTile();
	void setFinalDestinationTile(Tile* tile);

	//Lets delete enemy unit
	void cleanEnemyUnit();
protected:
	//Disables the old tiles selection (if ground tile) and
	//enables the newly selected tiles selection (if ground tile)
	void setSelectedTileIndex(int selectedIndex);
	GameObject* m_GameObj;
	//Protected Member variables
	Player* m_Player;
	Tile** m_Tiles;
	//Final destination tile for the enemies
	Tile* m_FinalTile;
	//Enemy Unit pointer 
	EnemyUnit* m_Mob;
	unsigned int m_HorizontalTiles;
	unsigned int m_VerticalTiles;
	unsigned int m_TileSize;
	unsigned int m_PlayerStartingTileIndex;
	//Final destination tile for the enemies
	unsigned int m_FinalDestinationTile;
	int m_SelectedTileIndex;
	bool m_PaintTileScoring;
	bool m_PaintBradPathScoring;
	bool m_PaintTileIndexes;
	mypq_type m_Queue;
	std::vector<Unit*> m_Units;
	SpawnPoint* m_SpawnPoint;
	//The Player lifes and GameOver state(DeadEnd)
	int m_LifeCounter;
	int m_LifeSpace;
	bool m_DeadEnd;
	OpenGLTexture* m_Life; 
	//Timer variable to delay reseting the game has ended
	double m_GameOverTimer;
};

#endif
