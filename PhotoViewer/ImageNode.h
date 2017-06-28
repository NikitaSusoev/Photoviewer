#pragma once
#include "node.h"
#include <QPoint>
class ImageNode :
	public Node
{
	QPixmap _pixm;
	QBrush _brushCanvas;
	

public:
	ImageNode(void);
	~ImageNode(void);

	QPixmap pixm() {return _pixm;}
	void setPixm(QPixmap pxm){_pixm = pxm;} 

	
	
	void paint(QPainter *painter, QPixmap pix);
};

