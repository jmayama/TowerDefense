#include "FastPathFinder.h"
#include "../Game/Tiles/Tile.h"
#include "../Game/Level.h"

#include <stdlib.h>
FastPathFinder::FastPathFinder(Level *level)
{
	m_Path = NULL;
	m_Level = level;
	m_Last = -1;
	int nodeCount = level->getNumberOfTiles();
	m_OpenList = new Node*[nodeCount];
	m_AllNodes = new Node*[nodeCount];
	int w = level->getNumberOfHorizontalTiles();
	int h = level->getNumberOfVerticalTiles();

	int nodeIndex = 0;
	for (int y = 0; y < h;y++)
	{
		for (int x = 0; x < w; x++)
		{
			Node *node = new Node(x,y,level->getTileForIndex(nodeIndex)->getWeight());
			m_AllNodes[nodeIndex++] = node;
			
		}
	}

}

void FastPathFinder::reset()
{
	int nodeCount = m_Level->getNumberOfTiles();
	for (int i = 0; i < nodeCount; i++)
	{
		m_AllNodes[i]->reset();
	}
	m_Last = -1;
	delete m_Path;
	m_Path = NULL;
	m_PathLength = -1;
}

FastPathFinder::~FastPathFinder()
{
	if (m_OpenList != NULL)
	{
		delete[] m_OpenList;
		m_OpenList = NULL;
	}
	if (m_AllNodes != NULL)
	{
		delete[] m_AllNodes;
		m_AllNodes = NULL;
	}
	if (m_Path != NULL)
	{
		delete m_Path;
		m_Path = NULL;
	}
}

Node* FastPathFinder::getNodeFromOpenList()
{
	Node *result = NULL;
	if(m_Last >=0) {
		result = m_OpenList[m_Last];
		m_Last--;
	}
	return result;

}
void FastPathFinder::insertNodeIntoOpenList(Node *node)
{
	int f = node->getFScore();
	node->setState(OPEN);
	// fast first check
	if (m_Last == -1 || m_OpenList[m_Last]->getFScore() > f)
	{
		m_Last++;
		m_OpenList[m_Last] = node;
		return;
	}
	for (int i = m_Last-1; i>= 0; i--)
	{
		if (m_OpenList[i]->getFScore() >= f)
		{
			memmove(m_OpenList+i+2,m_OpenList+i+1,sizeof(Node*) * (m_Last - i));
			m_OpenList[i+1] = node;
			m_Last++;
			return;
		}
	}
	memmove(m_OpenList+1,m_OpenList,sizeof(Node*) * (m_Last+1));
	m_OpenList[0] = node;
	m_Last++;
}

Node **FastPathFinder::getPathBetweenCoordinates(int sourceX, int sourceY, int destinationX, int destinationY)
{
	// calculate a path between source and dest.
	// add the source node to the openlist.
	Node *currentNode = getNode(sourceX, sourceY);
	for (int i = 0; i < (int)(m_Level->getNumberOfTiles()); i++)
	{
		m_AllNodes[i]->reset();
		m_AllNodes[i]->setTarget(destinationX,destinationY);
	}
	m_Last = -1;
	int x = 0;
	int y = 0;
	currentNode->setParent(currentNode);
	insertNodeIntoOpenList(currentNode);
	bool areWeThereYet = false;
	while (m_Last >= 0) {
		Node * node = getNodeFromOpenList();
		currentNode = node;
		moveNodeToClosedList(node);
		x = node->getX();
		y = node->getY();

		// are we there yet
		if (x == destinationX && y == destinationY)
		{ // yes. yes we are.
			areWeThereYet = true;
			break;
		}
		if (y > 0) 
		{
			Node *up = getNode(x,y-1);
			if (up->getState()!= CLOSED) {
				if (up->getState() == OPEN) 
				{
					int fOfUp = currentNode->getGScore()+up->getWeight()+up->getHScore();
					if (fOfUp < up->getFScore())
					{
						removeNodeFromOpenList(up);
						up->setParent(node);
						insertNodeIntoOpenList(up);
					}
				}
				else {
					up->setParent(node);
					insertNodeIntoOpenList(up);
				}
			}
		}
		if (y<(int)(m_Level->getNumberOfVerticalTiles()-1))
		{
			Node *down = getNode(x,y+1);
			if (down->getState()!= CLOSED) {
				if (down->getState() == OPEN) 
				{
					int fOfDown = currentNode->getGScore()+down->getWeight()+down->getHScore();
					if (fOfDown < down->getFScore())
					{
						removeNodeFromOpenList(down);
						down->setParent(node);
						insertNodeIntoOpenList(down);
					}
				}
				else {
					down->setParent(node);
					insertNodeIntoOpenList(down);
				}
			}
		}
		if (x > 0)
		{
			Node *left = getNode(x-1,y);
			if (left->getState()!= CLOSED) {
				if (left->getState() == OPEN) 
				{
					int fOfLeft = currentNode->getGScore()+left->getWeight()+left->getHScore();
					if (fOfLeft < left->getFScore())
					{
						removeNodeFromOpenList(left);
						left->setParent(node);
						insertNodeIntoOpenList(left);
					}
				}
				else
				{
					left->setParent(node);
					insertNodeIntoOpenList(left);
				}
			}
		}
		if (x <(int)(m_Level->getNumberOfHorizontalTiles()-1))
		{
			Node *right = getNode(x+1,y);
			if (right->getState()!= CLOSED) {
				if (right->getState() == OPEN) 
				{
					int fOfRight = currentNode->getGScore()+right->getWeight()+right->getHScore();
					if (fOfRight < right->getFScore())
					{
						removeNodeFromOpenList(right);
						right->setParent(node);
						insertNodeIntoOpenList(right);
					}
				}
				else 
				{
					right->setParent(node);
					insertNodeIntoOpenList(right);
				}
			}
		}
	}

	if (areWeThereYet)
	{
		// we have a destination node.  How far to get back to the source?
		int count = 0;
		Node *n = getNode(destinationX,destinationY);
		Node *source = getNode(sourceX,sourceY);
		while (n != source)
		{
			count++;
			n = n->getParent();
		}
		count++; // account for the start point
		// we have space for count of them.  repeat the loop, counting backwards
		m_Path = new Node*[count];
		m_PathLength = count;
		n = getNode(destinationX,destinationY);
		while (n!= source)
		{
			m_Path[--count] = n;
			n = n->getParent();
		}
		m_Path[0] = source;
		return m_Path;
	}
	return NULL;
}
void FastPathFinder::moveNodeToClosedList(Node *node)
{
	node->setState(CLOSED);

}
int FastPathFinder::getPathLength()
{
	return m_PathLength;
}
Node * FastPathFinder::getPathElement(int index)
{
	return m_Path[index];
}
Node* FastPathFinder::getNode(int x, int y)
{
	return m_AllNodes[m_Level->getTileIndexForCoordinates(x,y)];
}

void FastPathFinder::removeNodeFromOpenList(Node *node)
{
	for (int i = 0; i < m_Last; i++) {
		if (m_OpenList[i] == node)
		{ // we found the one we need to pave over.
			memmove(m_OpenList+i,m_OpenList+i+1,sizeof(Node*) * (m_Last - i));
			break;
		}
	}

	m_Last --;
}
