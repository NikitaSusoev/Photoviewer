#ifndef FRAMESWIDGET_H
#define FRAMESWIDGET_H

#include <QWidget>
#include <QList>
#include "Node.h"
#include <QScrollBar>
#include <QAction>
#include <QRubberBand>
#include "model.h"

class FramesWidget : public QWidget
{
	Q_OBJECT
		QList<Node *> _nodes;
		QPoint _pointPicture;
		int _pointMouseX;
		int _pointMouseY;
		int _pointPictureX;
		int _pointPictureY;
		QPixmap _pict;
		QScrollBar *_horScroll;
		int _startValueScroll;
		int _frameWidth;
		int _heightPix;
		int _widthPix;
		int _tempOffset;
		int _countCall;
		QPoint _contextPoint;
		QList <Model::Element> _currentElements;
		int _startValueNodeX;
		int _imageHeight;
		int _spaceWidth;
		int _pressedIndexNode;
		QPoint _origin;
		QRubberBand *_rubberBand;
		int _dxMouse;
		int _dyMouse;
		int _timerID;
		int _dx;
		bool _isStartTimer;
		QPoint _posMoveEvent;

public:
	FramesWidget(QWidget *parent);
	~FramesWidget();
	void setNodes(QList<Node *> lst);
	void setPixmap(QPixmap &pix);
	QString getFilenameSelectedPicture();
	void selectElements(QList <Model::Element> elements);
	void setFilenames(QList <Model::Element> elements);
	QPixmap getPixmapPicture(Model::Element element, int heig);
	bool compareElementsList(QList <Model::Element> elements);

private:
	void generateOffsetPicture(Node *node);
	int indexAfter();

public slots:
	void valueChangedHorScroll(int value);
	void modelChanged();
	void insertPictures();
	void deletePictures();

signals:
	void selectionChanged();
	
protected:
	void mouseReleaseEvent (QMouseEvent * event );
	void mouseMoveEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void paintEvent(QPaintEvent *event);
	void resizeEvent (QResizeEvent *event);
	void contextMenuEvent (QContextMenuEvent *event);
	void timerEvent(QTimerEvent *event);

};

#endif // FRAMESWIDGET_H
