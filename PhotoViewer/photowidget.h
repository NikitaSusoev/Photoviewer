#ifndef PHOTOWIDGET_H
#define PHOTOWIDGET_H

#include <QWidget>
#include <QPixmap>
#include "model.h"

namespace photoViewer {

	enum ViewMode {unitScale, fitScale, otherScale};
}

using namespace photoViewer;

class PhotoWidget : public QWidget
{
	Q_OBJECT
		QPixmap _pict;
		QPoint _pointPicture;
		int _pointMouseX;
		int _pointMouseY;
		int _pointPictureX;
		int _pointPictureY;
		float _currentScale;
		int _widthPix;
		int _heightPix;
		QSize _tempFrameSize;
		QPixmap _tempPix;
		QBrush _brushCanvas;
		ViewMode _viewMode ;
		Model::Element _currentElement;

public:
	PhotoWidget(QWidget *parent);
	~PhotoWidget();
	void setPixmap(const QPixmap &pxm);
	void showPicture(Model::Element element);
	void createCanvas();
	void setScaleMode(ViewMode mode);

public slots:

	void modelChanged();

protected:
	void paintEvent(QPaintEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseDoubleClickEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent *event);
	void resizeEvent (QResizeEvent *event);

private:
	
};

#endif // PHOTOWIDGET_H
