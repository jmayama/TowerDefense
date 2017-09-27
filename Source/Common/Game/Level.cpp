//
//  Level.cpp
//  GAM-1532 OSX Game
//
//  Created by Bradley Flood on 2013-03-07.
//  Copyright (c) 2013 Algonquin College. All rights reserved.
// Modified by Dan Lingman on 2013-11-09
// added support for additonal tiles types
// added clear method
//

#include "Level.h"
#include "Player.h"
#include "Tiles/Tile.h"
#include "Tiles/GroundTile.h"
#include "Tiles/GrassTile.h"
#include "Tiles/RockTile.h"
#include "Tiles/RoadTile.h"
#include "Tiles/TreeTile.h"
#include "Tiles/WaterTile.h"
#include "../Constants/Constants.h"
#include "../Input/Input.h"
#include "../Screen Manager/ScreenManager.h"
#include <stdlib.h>
#include <fstream>
#include "../Libraries/jsoncpp/json.h"
#include "../Path/FastPathFinder.h"
#include "EnemyUnit.h"
//might use coupling for a variable that is used in most of class (check getInstance for openGL).
Level::Level(bool isEditingLevel) :
m_HorizontalTiles(0),
	m_VerticalTiles(0),
	m_TileSize(EMPTY_LEVEL_TILE_SIZE),
	m_PlayerStartingTileIndex(700), //(768/2+16) <-before  EMPTY_LEVEL_STARTING_PLAYER_TILE_INDEX), <---default players position.
	m_FinalDestinationTile(500),
	m_Player(NULL),
	m_Tiles(NULL),
	m_SelectedTileIndex(-1),
	m_PaintTileScoring(false),
	m_PaintTileIndexes(false),
	m_PaintBradPathScoring(false),
	m_Mob(NULL),
	m_LifeCounter(2),
	m_LifeSpace(0),
	m_DeadEnd(false),
	m_Life(NULL)
{
	//Create the player object
	if(isEditingLevel == false || true)
	{
		m_Player = new Player(this);
	}
	//Calculate the number of horizontal and vertical tiles
	m_HorizontalTiles = (unsigned int)(ScreenManager::getInstance()->getScreenWidth() / m_TileSize);
	m_VerticalTiles = (unsigned int)(ScreenManager::getInstance()->getScreenHeight() / m_TileSize);

	//Allocate the tiles array, the inheriting class will populate this array with Tile objects
	m_Tiles = new Tile*[m_HorizontalTiles * m_VerticalTiles];

	//Initialize all the tiles to NULL
	for(int i = 0; i < (int)(m_HorizontalTiles * m_VerticalTiles); i++)
	{
		m_Tiles[i] = NULL;
	}

	m_SpawnPoint = new SpawnPoint(this);
	
	//Load an empty level
	load(NULL);
	
}

Level::~Level()
{
	//Delete the player object
	if(m_Player != NULL)
	{
		delete m_Player;
		m_Player = NULL;
	}

	if(m_Mob != NULL)
	{

		delete m_Mob;
		m_Mob = NULL;
	}

	//Delete the tiles array, the inheriting class
	//must delete the object in this array itself
	if(m_Tiles != NULL)
	{
		//Cycle through and delete all the tile objects in the array
		for(int i = 0; i < (int)getNumberOfTiles(); i++)
		{
			if(m_Tiles[i] != NULL)
			{
				delete m_Tiles[i];
				m_Tiles[i] = NULL;
			}
		}

		delete[] m_Tiles;
		m_Tiles = NULL;
	}
}

void Level::clear()
{

	for(int i = 0; i < (int)getNumberOfTiles(); i++)
	{
		if(m_Tiles[i] != NULL)
		{
			setTileTypeAtIndex(TileTypeGround,i);
		}
	}
}
//Enemy unit are going to the playerTile
Tile* Level::getPlayerTile()  
{
	return m_Player->getCurrentTile();
}

Tile* Level::getFinalDestinationTile()
{

	return m_FinalTile;
}
void Level::setFinalDestinationTile(Tile* tile)
{
	m_FinalTile = tile;
	//m_GameObj->setPosition(tile->getX()+800, tile->getY()+7);
	tile = m_Tiles[m_FinalDestinationTile];

}
void Level::update(double aDelta)
{
	//Update all the game tiles
	for(int i = 0; i < (int)getNumberOfTiles(); i++)
	{
		if(m_Tiles[i] != NULL)
		{
			m_Tiles[i]->update(aDelta);
		}
	}

	if (m_Queue.empty() == false)
	{
		Unit *unit = m_Queue.top();
		m_Queue.pop();
		unit->setState(START);
	}

	//Update the Player
	if(m_Player != NULL)
	{
		m_Player->update(aDelta);

	}
	if (m_SpawnPoint != NULL)
	{
		m_SpawnPoint->update(aDelta);
	}

	for (int i = 0; i < m_Units.size(); i++)
	{
		Unit* unit = (Unit*)(m_Units.at(i));
		unit->update(aDelta);

	}
}

void Level::paint()
{
	m_Mob->paintLife();

	if(m_DeadEnd) 
	{
		ScreenManager::getInstance()->switchScreen(GAMEOVER_MENU_SCREEN_NAME);

	}
	else 
	{

		//Cycle through and paint all the tiles
		for(int i = 0; i < (int)getNumberOfTiles(); i++)
		{
			//Safety check the tile
			if(m_Tiles[i] != NULL)
			{
				//Paint the tile
				m_Tiles[i]->paint();

				//If the paint tile indexes flag is set to true,
				//draw the index number on the tiles
				if(m_PaintTileIndexes == true)
				{
					m_Tiles[i]->paintIndex(i);
				}
			}
		}


		//Paint the Player
		if(m_Player != NULL)
		{
			//If paint tile scoring flag is set to true,
			//draw the path scoring

			if(m_PaintTileScoring == true)
			{
				for (int x = 0; x < (int)getNumberOfHorizontalTiles(); x ++)
				{
					for (int y = 0; y < (int)getNumberOfVerticalTiles(); y++)
					{
						Node *node = m_Player->getPathFinder()->getNode(x,y);
						if (node->getState() != RAW) 
						{
							Tile *tile = getTileForCoordinates(x,y);
							tile->paintScoreF(node->getFScore());
						}
					}
				}
				for (int index = 0; index < m_Player->getPathFinder()->getPathLength();index++)
				{
					Node *n = m_Player->getPathFinder()->getPathElement(index);
					OpenGLColor old = OpenGLRenderer::getInstance()->getForegroundColor();
					OpenGLRenderer::getInstance()->setForegroundColor(OpenGLColorRed());
					OpenGLRenderer::getInstance()->drawRectangle((float)(n->getX()*32),(float)(n->getY()*32),32.0f,32.0f,false);
					OpenGLRenderer::getInstance()->setForegroundColor(old);

				}
				//TODO: Paint the tile scores

			}	
			//Paint the player
			m_Player->paint();
		}

		for (int i = 0; i < m_LifeCounter; i++)
		{
			OpenGLRenderer::getInstance()->drawTexture(m_Life, m_LifeSpace, 700.0f);
			m_LifeSpace = m_LifeSpace + 60;
		}

		for (int i = 0; i < m_Units.size(); i++)
		{
			Unit* unit = (Unit*)(m_Units.at(i));
			unit->paint();

		}

		m_SpawnPoint->paint();
	}

}

void Level::reset()
{
	//Cycle through and reset all the tiles
	for(int i = 0; i < (int)getNumberOfTiles(); i++)
	{
		if(m_Tiles[i] != NULL)
		{
			m_Tiles[i]->reset();
		}
	}

	//Reset the Player
	if(m_Player != NULL)
	{
		m_Player->reset();
		m_Player->setCurrentTile(m_Tiles[m_PlayerStartingTileIndex]);
	}
	for (int i = 0; i < m_Units.size(); i++)
	{
		Unit* unit = (Unit*)(m_Units.at(i));
		unit->reset();
		switch(i)
		{
		case 0:
			unit->setCurrentTile(m_Tiles[0]);
			unit->setDestinationTile(m_Tiles[m_PlayerStartingTileIndex]);//(m_Tiles[m_PlayerStartingTileIndex]); 
			break;
		case 1:
			unit->setCurrentTile(m_Tiles[30]);
			unit->setDestinationTile(m_Tiles[m_PlayerStartingTileIndex]);//(m_Tiles[m_PlayerStartingTileIndex]);
			break;
		}
	}
	m_SpawnPoint->setCurrentTile(m_Tiles[0]); //default SpawnPoint


}


void Level::mouseLeftClickUpEvent(float aPositionX, float aPositionY)
{
	//Convert the mouse click position, into a tile index
	int index = getTileIndexForPosition((int)aPositionX, (int)aPositionY);

	//Safety check that the tile isn't NULL
	if(m_Tiles[index] != NULL)
	{
		//Set the selected tile index
		setSelectedTileIndex(index);

		//If the tile is walkable, set the player's destination tile
		if(m_Tiles[index]->isWalkableTile() == true)
		{
			if(m_Player != NULL)
			{
				m_Player->setDestinationTile(m_Tiles[m_SelectedTileIndex]);
			}
		}
	}
}

void Level::keyUpEvent(int keyCode)
{
	if(keyCode == KEYCODE_R)
	{
		reset();
	}
	else if(keyCode == KEYCODE_S)
	{
		togglePaintTileScoring();
	}
	else if(keyCode == KEYCODE_I)
	{
		togglePaintTileIndexes();
	}
}

void Level::load(const char* levelName)
{
	//If the level name isn't NULL load the level from the filesystem,
	//if it is NULL load an empty level with just ground tiles
	if(levelName != NULL)
	{
		Json::Value root;
		Json::Reader reader;
		std::ifstream in;
		in.open(levelName);
		bool success = reader.parse(in,root,false);
		in.close();
		Json::Value tiles = root["tiles"];
		for(int i = 0; i < (int)(tiles.size()); i++)
		{
			int tileType = tiles[i].get("tileType",NULL).asInt();
			setTileTypeAtIndex((TileType)tileType,i);
		}
	}
	else
	{
		//Tile variables
		int tileIndex = 0;
		float tileX = 0.0f;
		float tileY = 0.0f;

		//Cycle through all the tiles and create them
		for(int v = 0; v < (int)getNumberOfVerticalTiles(); v++)
		{
			for(int h = 0; h < (int)getNumberOfHorizontalTiles(); h++)
			{
				//The empty level will contain only ground tiles
				m_Tiles[tileIndex] = new GroundTile();
				m_Tiles[tileIndex]->setPosition(tileX, tileY);
				m_Tiles[tileIndex]->setSize((float)m_TileSize,(float) m_TileSize);

				//Increment the tile index
				tileIndex++;

				//And increment the tile x position
				tileX += m_TileSize;
			}

			//Increment the tile y position and reset the tile x position, since we started a new row
			tileY += m_TileSize;
			tileX = 0.0f;
		}
	}

	//The level is loaded, reset everything
	reset();
}

void Level::save(const char* levelName)
{
	Json::Value root;
	Json::Value tiles;
	for(int i = 0; i < (int)getNumberOfTiles(); i++)
	{
		Json::Value tileType;
		tileType["tileType"] = getTileTypeForIndex(i);
		tiles.append(tileType);
	}
	root["tiles"] = tiles;
	std::ofstream out;
	out.open(levelName);
	out << root;
	out.close();
}

TileType Level::getTileTypeForIndex(int index)
{
	if(index >= 0 && index < (int)getNumberOfTiles())
	{
		return m_Tiles[index]->getTileType();
	}
	return TileTypeUnknown;
}

unsigned int Level::getNumberOfTiles()
{
	return getNumberOfHorizontalTiles() * getNumberOfVerticalTiles();
}

unsigned int Level::getNumberOfHorizontalTiles()
{
	return m_HorizontalTiles;
}

unsigned int Level::getNumberOfVerticalTiles()
{
	return m_VerticalTiles;
}

bool Level::validateTileCoordinates(int aCoordinatesX, int aCoordinatesY)
{
	if(aCoordinatesX < 0 || aCoordinatesY < 0 || aCoordinatesX >= (int)getNumberOfHorizontalTiles() || aCoordinatesY >= (int)getNumberOfVerticalTiles())
	{
		return false;
	}
	return true;
}

int Level::getTileCoordinateForPosition(int aPosition)
{
	return (aPosition / m_TileSize);
}

int Level::getTileIndexForPosition(int aPositionX, int aPositionY)
{
	int coordinatesX = getTileCoordinateForPosition(aPositionX);
	int coordinatesY = getTileCoordinateForPosition(aPositionY);
	return getTileIndexForCoordinates(coordinatesX, coordinatesY);
}

int Level::getTileIndexForCoordinates(int aCoordinatesX, int aCoordinatesY)
{
	//Validate the coordinates, then calculate the array index
	if(validateTileCoordinates(aCoordinatesX, aCoordinatesY) == true)
	{
		return aCoordinatesX + (aCoordinatesY * getNumberOfHorizontalTiles());
	}

	return -1;
}

int Level::getTileIndexForTile(Tile* aTile)
{
	return getTileIndexForPosition((int)(aTile->getX()), (int)(aTile->getY()));
}

Tile* Level::getTileForPosition(int aPositionX, int aPositionY)
{
	return getTileForIndex(getTileIndexForPosition(aPositionX, aPositionY));
}

Tile* Level::getTileForCoordinates(int aCoordinatesX, int aCoordinatesY)
{
	return getTileForIndex(getTileIndexForCoordinates(aCoordinatesX, aCoordinatesY));
}

Tile* Level::getTileForIndex(int aIndex)
{
	//Safety check the index bounds
	if(aIndex >= 0 && aIndex < (int)getNumberOfTiles())
	{
		return m_Tiles[aIndex];
	}

	//If we got here, it means the index passed in was out of bounds
	return NULL;
}

void Level::setTileTypeAtPosition(TileType tileType, int positionX, int positionY)
{
	setTileTypeAtIndex(tileType, getTileIndexForPosition(positionX, positionY));
}

void Level::setTileTypeAtCoordinates(TileType tileType, int coordinatesX, int coordinatesY)
{
	setTileTypeAtIndex(tileType, getTileIndexForCoordinates(coordinatesX, coordinatesY));
}

void Level::setTileTypeAtIndex(TileType tileType, int index)
{
	//Safety check the index
	if(index >= 0 && index < (int)getNumberOfTiles())
	{
		//Don't replace the tile if its the same type of tile that already exists
		if(m_Tiles[index]->getTileType() == tileType)
		{
			return;
		}

		//Delete the tile at the index, if one exists
		if(m_Tiles[index] != NULL)
		{
			delete m_Tiles[index];
			m_Tiles[index] = NULL;
		}

		//Create the new tile based on the TileType
		switch (tileType)
		{
		case TileTypeGround:
			m_Tiles[index] = new GroundTile();
			break;
		case TileTypeGrass:
			m_Tiles[index] = new GrassTile();
			break;
		case TileTypeRoad:
			m_Tiles[index] = new RoadTile();
			break;
		case TileTypeRock:
			m_Tiles[index] = new RockTile();
			break;
		case TileTypeTree:
			m_Tiles[index] = new TreeTile();
			break;
		case TileTypeWater:
			m_Tiles[index] = new WaterTile();
			break;
		case TileTypeUnknown:
		default:
			m_Tiles[index] = NULL;
			break;
		}

		//Calculate the coordinates and set the tile position and size
		int coordinateX = (index % getNumberOfHorizontalTiles());
		int coordinateY = ((index - coordinateX) / getNumberOfHorizontalTiles());
		m_Tiles[index]->setPosition((float)(coordinateX  * m_TileSize), (float)(coordinateY * m_TileSize));
		m_Tiles[index]->setSize((float)m_TileSize, (float)m_TileSize);
	}
}

void Level::togglePaintTileScoring()
{
	m_PaintTileScoring = !m_PaintTileScoring;
}

void Level::togglePaintTileIndexes()
{
	m_PaintTileIndexes = !m_PaintTileIndexes;
}

void Level::setSelectedTileIndex(int aSelectedIndex)
{
	//Deselect the previously selected tile
	if(m_SelectedTileIndex >= 0 && m_SelectedTileIndex < (int)getNumberOfTiles())
	{
		m_Tiles[m_SelectedTileIndex]->setIsSelected(false);
	}

	//Set the new tile index
	m_SelectedTileIndex = aSelectedIndex;

	//Selected the newly selected tile
	if(m_SelectedTileIndex >= 0 && m_SelectedTileIndex < (int)getNumberOfTiles())
	{
		m_Tiles[m_SelectedTileIndex]->setIsSelected(true);
	}
}

void Level::queueForPathFinding (Unit * unit)
{
	unit->setState(STATIONARY);
	m_Queue.push(unit);
}

void Level::addEnemyUnit(Unit *unit)
{
	m_Units.push_back(unit);
	//NOW THIS IS WHERE THEY ARE GOING!!!
	unit->setDestinationTile(m_Tiles[m_FinalDestinationTile]);//unit->setDestinationTile(m_Player->getCurrentTile());
	queueForPathFinding(unit);
}

//This function is used to erase the mob that reached its destination
//Its called in EnemyUnit class the reachedDestination() function 
void Level::cleanEnemyUnit()
{	
	for (int i = 0; i < m_Units.size(); i--)
	{
		//The expression values.begin() returns an iterator pointing to the first element of the vector
		//the index "i" kept decreasing so I added to begin to erase the 1st element reaching the goal
		m_Units.erase(m_Units.begin() + i);

	}

}

void Level::gameOver()
{
	
	if(m_LifeCounter <= 1)
	{
		//char choose = 'r';
		m_DeadEnd = true;

		//while(!choose){

		m_GameOverTimer = GAME_OVER_TIMER;

		//keyUpEvent(choose); //the option would be to reset the game...

		//}
		//Default lives.
		m_LifeCounter = 1;

	}

	//We still have game lives.
	else
	{   
		--m_LifeCounter;
		//a reset?

	}

};