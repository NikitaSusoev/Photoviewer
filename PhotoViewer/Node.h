#pragma once
#include <QPoint>
#include <QSize>
#include <QPainter>



class Node
{
	QPoint _pos;
	QSize _size;
	bool _isSelect;
	QString _filename;
	int _indexNode;
	int _indexFrame;
	bool _isDragging;

public:
	Node(void);
	~Node(void);
	
	bool isDragging() {return _isDragging;}
	void setDragging(bool dragging){_isDragging = dragging;}

	QPoint pos() {return _pos;}
	void setPos(QPoint pnt){_pos = pnt;} 
	void setPos(int x, int y){_pos = QPoint(x,y);} 
	QSize size() {return _size;}
	void setSize(QSize sz){_size = sz;}
	void setSize(int x, int y){_size = QSize(x,y);}
	bool isSelect(){return _isSelect;}
	void setSelection(bool slct){_isSelect = slct;}
	QString filename(){return _filename;}
	void setFilename(QString flnm){_filename = flnm;}
	int indexFrame(){return _indexFrame;}
	void setIndexFrame(int flnm){_indexFrame = flnm;}
	int indexNode(){return _indexNode;}
	void setIndexNode(int index){_indexNode = index;}

	virtual void paint(QPainter *painter, QPixmap pix);
};

