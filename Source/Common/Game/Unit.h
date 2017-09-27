//
//  Unit.h
//  GAM-1532 OSX Game
//
//  Created by Bradley Flood on 2013-03-07.
//  Copyright (c) 2013 Algonquin College. All rights reserved.
//

#ifndef UNIT_H
#define UNIT_H

#include "GameObject.h"
#include <vector>

class Level;
class Tile;
class FastPathFinder;

enum PathState {START=0, ABOUT_TO_MOVE, MOVING, AT_NEXT_NODE, AT_DESTINATION, OBSTACLE,STATIONARY};
class Unit : public GameObject
{
public:
	Unit(Level* level);
	virtual ~Unit();

	//Update, paint and reset methods
	virtual void update(double delta);
	virtual void paint();
	virtual void reset();

	//Implementing GameObject's pure virtual method
	virtual const char* getType()=0;

	//Setter methods for the current and destination tiles
	void setCurrentTile(Tile* tile);
	void setDestinationTile(Tile* tile);
	Tile* getDestinationTile(); //getter method for the destination tiles
	void getCoordinatesFromTile(Tile* tile,int &x,int &y);
	FastPathFinder *getPathFinder();
	float getScore();
	void setScore(float score);
	void setState(PathState state);
	void deleteRequested();
	virtual void reachedDestination();
protected:
	//Animation methods
	float animate(float current, float target, double delta);
	void startAnimating();
	void stopAnimating();
	bool isAnimating();
	
protected:
	Tile* m_CurrentTile;
	Tile* m_NextTile;
	Tile* m_DestinationTile;
	bool m_CanAnimate;
	bool m_AbortAnimation;
	int m_AnimationPathNodeIndex;
	FastPathFinder *m_FastPathFinder;
	Level* m_Level;
	PathState m_State;
	int m_PathIndex;

	float m_Score;

	// animation data

	float m_SX;
	float m_SY;
	float m_TX;
	float m_TY;
	double m_ElapsedTime;
	double m_Duration;
	bool m_StartRequested;
	bool m_DeleteRequested;
	bool m_ResetRequested;
};

#endif