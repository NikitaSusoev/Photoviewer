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
	QPixmap tempPix = pixm();
	QPoint posFrame(pos().x() - offsetFrame, pos().y() - offsetFrame);
	QRect rect;
	QSize sizeFrame;
	if (!this->isDragging()){	
		sizeFrame.setWidth(size().width() + 2*offsetFrame);
		sizeFrame.setHeight(size().height() + 2*offsetFrame);
		rect.setRect(posFrame.x(), posFrame.y(), sizeFrame.width(), sizeFrame.height());

	}

	if (this->isDragging()){
		painter->setOpacity(1);
		tempPix = tempPix.scaled(30, 30, Qt::KeepAspectRatio);
		sizeFrame.setWidth(tempPix.rect().width() + 2*offsetFrame);
		sizeFrame.setHeight(tempPix.rect().height() + 2*offsetFrame);
		rect.setRect(posFrame.x(), posFrame.y(), sizeFrame.width(), sizeFrame.height());
	} else {
		painter->setOpacity(1);
	}

	if (Model::get()->isSelected(this->indexNode()))
	{
		painter->setBrush(QBrush(Qt::black));
	}
	else
	{
		painter->setBrush(QBrush(Qt::NoBrush));
	}

	painter->setPen(QPen(Qt::lightGray));
	painter->drawRect(rect);

		if (tempPix.hasAlphaChannel())
		{
				QRect rectCanvas;

			if (this->isDragging())
			{
				rectCanvas.setRect(pos().x(),pos().y(),tempPix.rect().width(),tempPix.rect().height());
			}else
			{
				rectCanvas.setRect(pos().x(),pos().y(),size().width(),size().height());
			}
			
			painter->fillRect(rectCanvas, _brushCanvas);
		}

	painter->drawPixmap(pos(),tempPix);
}
