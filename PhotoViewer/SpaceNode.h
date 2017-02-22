#pragma once
#include "node.h"
class SpaceNode :
	public Node
{
public:
	SpaceNode(void);
	~SpaceNode(void);

	void paint(QPainter *painter, QPixmap pix);
};

