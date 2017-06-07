#pragma once
#include "node.h"
#include <QPoint>
class ImageNode :
	public Node
{
	QPixmap _pixm;
	QBrush _brushCanvas;
	QPoint _startPoint;

public:
	ImageNode(void);
	~ImageNode(void);

	QPixmap pixm() {return _pixm;}
	void setPixm(QPixmap pxm){_pixm = pxm;} 

	void setStartPoint(QPoint point){_startPoint = point;}
	QPoint startPoint(){return _startPoint;}
	
	void paint(QPainter *painter, QPixmap pix);
};

