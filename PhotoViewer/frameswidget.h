#ifndef FRAMESWIDGET_H
#define FRAMESWIDGET_H

#include <QWidget>
#include <QList>
#include "Node.h"
#include <QScrollBar>
#include <QAction>

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
		QStringList _currentLst;

public:
	FramesWidget(QWidget *parent);
	~FramesWidget();
	void setNodes(QList<Node *> lst);
	void setPixmap(QPixmap &pix);
	QString getFilenameSelectedPicture();
	void selectFilename(QString newFilename);
	void setFilenames(QStringList lst);
	int getWidthPicture(QString filename, int heig);
	QPixmap getPixmapPicture(QString filename, int heig);

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
	void paintEvent(QPaintEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void resizeEvent (QResizeEvent *event);
	void contextMenuEvent (QContextMenuEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
};

#endif // FRAMESWIDGET_H
