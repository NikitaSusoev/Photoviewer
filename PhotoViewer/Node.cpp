#include "Node.h"


Node::Node(void)
{
}


Node::~Node(void)
{
}

void Node::paint(QPainter *painter, QPixmap pix)
{
	QRect rect(pos(),size());
	painter->setBrush(QBrush(Qt::black));
	painter->drawRect(rect);
}
