#include "SpaceNode.h"


SpaceNode::SpaceNode(void)
{
}


SpaceNode::~SpaceNode(void)
{
}

void SpaceNode::paint(QPainter *painter, QPixmap pix)
{
	QRect rect(pos(),size());

	painter->setBrush(QBrush(Qt::NoBrush));
	painter->setPen(QPen(Qt::NoPen));
	painter->drawRect(rect);
}
