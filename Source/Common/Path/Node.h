#ifndef NODE_H
#define NODE_H
enum NodeState {RAW=0,OPEN,CLOSED};
class Node
{
public:
	

	Node(int x, int y, int w);
	~Node();
	void setParent(Node *parent);
	int getGScore();
	int getHScore();
	int getFScore();
	NodeState getState();
	void setState(NodeState state);
	void setTarget(int x, int y);
	void setWeight(int weight);
	int getWeight();
	void reset();
	int getX();
	int getY();

	Node *getParent();
private:
	Node *m_Parent;
	int m_X;
	int m_Y;
	int m_G;
	int m_H;
	int m_TargetX;
	int m_TargetY;
	int m_Weight;
	NodeState m_State;
};


#endif