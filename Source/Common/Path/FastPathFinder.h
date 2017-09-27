#ifndef FAST_PATH_FINDER_H
#define FAST_PATH_FINDER_H
#include "Node.h"

class Level;

class FastPathFinder
{
public:
	FastPathFinder(Level *level);
	~FastPathFinder();

	Node* getNodeFromOpenList();
	void insertNodeIntoOpenList(Node *node);
	Node **getPathBetweenCoordinates(int sourceX, int sourceY, int destinatonX, int destinationY);
	void moveNodeToClosedList(Node *node);
	Node* getNode(int x, int y);
	void removeNodeFromOpenList(Node *node);
	int getPathLength();
	Node * getPathElement(int index);
	void reset();
private:
	Node** m_OpenList;
	int m_Last;
	int m_DestinationX;
	int m_DestinationY;
	int m_StartX;
	int m_StartY;
	Level *m_Level;
	Node** m_Path;
	int m_PathLength;
	Node** m_AllNodes;
};


#endif