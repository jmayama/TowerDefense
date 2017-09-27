#include "Node.h"
#include <math.h>
#include <stdlib.h>

Node::Node(int x, int y, int w)
{
	m_X = x;
	m_Y = y;
	m_Weight = w;
	m_State = RAW;
	m_H = 0;
	m_G = 0;
	m_Parent = NULL;
}

Node::~Node()
{
}

void Node::setParent(Node *parent)
{
	m_Parent = parent;
	m_G = m_Parent->getGScore()+m_Weight;
}

Node *Node::getParent()
{
	return m_Parent;
}


int Node::getGScore()
{
	return m_G;
}
int Node::getHScore()
{
	return m_H;
}
int Node::getFScore()
{
	return m_G+m_H;
}
NodeState Node::getState()
{
	return m_State;
}

int Node::getWeight()
{
	return m_Weight;
}

void Node::setState(NodeState state)
{
	m_State = state;
}

void Node::setTarget(int x, int y)
{
	m_TargetX = x;
	m_TargetY = y;
	m_H = 1*(abs(x-m_X)+abs(y-m_Y));
}

int Node::getX()
{
	return m_X;
}

int Node::getY()
{
	return m_Y;
}

void Node::setWeight(int weight)
{
	m_Weight = weight;
}

void Node::reset()
{
	setTarget(m_X,m_Y);
	m_Parent = NULL;
	m_G = 0;
	m_State = RAW;
}