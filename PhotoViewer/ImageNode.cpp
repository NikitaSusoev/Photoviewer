#include "ImageNode.h"
#include "photoviewer.h"
#include "model.h"

ImageNode::ImageNode(void)
{
}


ImageNode::~ImageNode(void)
{
}

void ImageNode::paint(QPainter *painter, QPixmap pix)
{
	int offsetFrame = 3;
	QPoint posFrame(pos().x() - offsetFrame, pos().y() - offsetFrame);
	QSize sizeFrame(size().width() + 2*offsetFrame, size().height() + 2*offsetFrame);
	QRect rect(posFrame,sizeFrame);

	if (Model::get()->isSelected(indexNode()))
	{
		painter->setBrush(QBrush(Qt::black));
	} 
	else
	{
		painter->setBrush(QBrush(Qt::NoBrush));
	}	

	painter->setPen(QPen(Qt::lightGray));
	painter->drawRect(rect);
	painter->drawPixmap(pos(),pixm());
}
