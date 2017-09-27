//
//  EnemyUnit.cpp
//  GAM-1532 OSX Game
//
//  Created by Bradley Flood on 2013-03-07.
//  Copyright (c) 2013 Algonquin College. All rights reserved.
//

#include "EnemyUnit.h"
#include "Level.h"
#include "Tiles/Tile.h"
#include "../OpenGL/OpenGL.h"
#include "../Constants/Constants.h"
#include <stdlib.h>
#include <algorithm>
#include <math.h>
#include "../Path/FastPathFinder.h"
#include "../Utils/Logger/Logger.h"
#include "../Screen Manager/ScreenManager.h"

EnemyUnit::EnemyUnit(Level* aLevel):Unit(aLevel)
{
	setScore(10);
	m_LifeCounter = 0;
	m_LifeSpace = 0;
	m_DeadEnd = false;
	m_Life = new OpenGLTexture(GAME_LIFE_TEXTURE);
	

}

EnemyUnit::~EnemyUnit()
{
	if(m_Life != NULL)
	{
		delete m_Life;
		m_Life = NULL;
	}

}

const char* EnemyUnit::getType()
{
	return ENEMY_TYPE;
}

void EnemyUnit::paint()
{

	OpenGLRenderer::getInstance()->setForegroundColor(OpenGLColorRed());
	OpenGLRenderer::getInstance()->drawCircle(getX() + (getWidth() / 2), getY() + (getHeight() / 2), getWidth() / 2,true,12);
	OpenGLRenderer::getInstance()->setForegroundColor(PLAYER_OUTLINE_COLOR);
	OpenGLRenderer::getInstance()->drawCircle(getX() + (getWidth() / 2), getY() + (getHeight() / 2), getWidth() / 2, false,12);
	// eyes
	OpenGLRenderer::getInstance()->setForegroundColor(OpenGLColorRed());
	OpenGLRenderer::getInstance()->drawCircle(getX() + (getWidth() / 2)-10, getY() + (getHeight() / 2)-5, 4, true,12);
	OpenGLRenderer::getInstance()->setForegroundColor(PLAYER_OUTLINE_COLOR);
	OpenGLRenderer::getInstance()->setForegroundColor(OpenGLColorRed());
	OpenGLRenderer::getInstance()->drawCircle(getX() + (getWidth() / 2)+10, getY() + (getHeight() / 2)-5, 4, true,12);
	OpenGLRenderer::getInstance()->setForegroundColor(PLAYER_OUTLINE_COLOR);

	//this is where the eyes are following the player...
	Tile* player = m_Level->getPlayerTile();
	float eyeX;
	float eyeY;
	float px,py;
	float ex = getX();
	float ey = getY();
	player->getPosition(px,py);

	int dx = (int)(px-ex);
	int dy = (int)(py-ey);
	if (dx == 0)
	{
		eyeX = 0;
	}
	else if (dx < 0)
	{
		eyeX = -2;
	}
	else // dx > 0
	{
		eyeX = +2;
	}
	if (dy == 0)
	{
		eyeY = 0;
	}
	else if (dy < 0)
	{
		eyeY = -2;
	}
	else
	{
		eyeY = +2;
	}
	//Pupil
	OpenGLRenderer::getInstance()->setForegroundColor(OpenGLColorBlack());
	OpenGLRenderer::getInstance()->drawCircle(getX() + (getWidth() / 2)-10+eyeX, getY() +eyeY+ (getHeight() / 2)-5, 1, true,12);
	OpenGLRenderer::getInstance()->drawCircle(getX() + (getWidth() / 2)+10+eyeX, getY() +eyeY+ (getHeight() / 2)-5, 1, true,12);


}

void EnemyUnit::paintLife()
{

	for (int i = 0; i < m_LifeCounter; i++)
	{
		OpenGLRenderer::getInstance()->drawTexture(m_Life, m_LifeSpace, 700.0f);
		m_LifeSpace = m_LifeSpace + 60;
	}

}

void EnemyUnit::reachedDestination()
{


	//By doing this it aknowledge if it reached the final destination
	m_CurrentTile = m_Level->getFinalDestinationTile();

	//or m_Level->getPlayerTile() but nah. 
	if (m_CurrentTile == m_Level->getFinalDestinationTile()) {
		// we're done!
		// -HEART->GAME OVER MAN... 

		//Will erase the mob that reached the destination tile
		m_Level->cleanEnemyUnit();


		while(m_LifeCounter >= 0)
		{
			--m_LifeCounter;

			if(m_LifeCounter == 0)
			{
				ScreenManager::getInstance()->switchScreen(GAMEOVER_MENU_SCREEN_NAME);
			}
		}
		//m_Level->gameOver();

		//ScreenManager::getInstance()->switchScreen(MAIN_MENU_SCREEN_NAME); //GAMEOVER_MENU_SCREEN_NAME

	}
	setScore(10);
	//When I get rid of one of the 2 line of code below: 1 line by itself will crash, 2nd line by itself will not crash. if both line stay it crashes
	m_DestinationTile = m_Level->getFinalDestinationTile(); //m_Level->getPlayerTile(); //Here that enemy chases after player no matter what.
	m_Level->queueForPathFinding(this);

}

