#include "ImageNode.h"
#include "photoviewer.h"
#include "model.h"

ImageNode::ImageNode(void)
{
	QImage img(2,2,QImage::Format_RGB32);
	img.setPixel(0,0,qRgb(150,150,150));
	img.setPixel(0,1,qRgb(255,255,255));
	img.setPixel(1,0,qRgb(255,255,255));
	img.setPixel(1,1,qRgb(150,150,150));

	img = img.scaled(40,40);

	_brushCanvas.setTextureImage(img);
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

	if (pixm().hasAlphaChannel())
	{
		QPoint posCanvas(pos().x(), pos().y());
		QSize sizeCanvas(size().width(), size().height());
		QRect rectCanvas(posCanvas,sizeCanvas);
		painter->fillRect(rectCanvas,_brushCanvas);
	}

	painter->drawPixmap(pos(),pixm());


}
