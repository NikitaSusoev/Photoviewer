#include "photowidget.h"
#include <QPainter>
#include <QMouseEvent>
#include "photoviewer.h"
#include "model.h"

#include "webp/decode.h"
#include "webp/encode.h"
#include "webp/types.h"

PhotoWidget::PhotoWidget(QWidget *parent)
	: QWidget(parent)
{	
	_currentScale = 1; 
	connect(Model::get(), SIGNAL(modelChanged()), this, SLOT(modelChanged()));
	createCanvas();
	_viewMode = fitScale;
}

PhotoWidget::~PhotoWidget()
{
	
}

void PhotoWidget::createCanvas()
{
	QImage img(2,2,QImage::Format_RGB32);
	img.setPixel(0,0,qRgb(150,150,150));
	img.setPixel(0,1,qRgb(255,255,255));
	img.setPixel(1,0,qRgb(255,255,255));
	img.setPixel(1,1,qRgb(150,150,150));

	img = img.scaled(40,40);

	_brushCanvas.setTextureImage(img);


}


void PhotoWidget::setScaleMode(ViewMode mode)
{
	switch (mode)
	{
	case unitScale:
		_viewMode = unitScale;
		break;

	case fitScale:
		_viewMode = fitScale;
		break;

	case otherScale:
		_viewMode = otherScale;
		break;
	}
	update();
}

void PhotoWidget::setPixmap(const QPixmap &pxm)
{
	_pict = pxm;

	if (this->frameSize().width()>_pict.width() && this->frameSize().height()>_pict.height())
	{
		_viewMode = unitScale;

	} else if (this->frameSize().width()<_pict.width() && this->frameSize().height()<_pict.height())
	{
		_viewMode = fitScale;
	} 
	else
	{

	}

	update();
}

void PhotoWidget::showPicture(Model::Element element)
{
	setWindowTitle(element.filename);
	setPixmap(Model::get()->getPixmapFromElement(element));
}


void PhotoWidget::modelChanged()
{
	if (Model::get()->selectedElements().count() == 1)
	{
		showPicture(Model::get()->selectedElements().first());
	}	
}

void PhotoWidget::paintEvent(QPaintEvent *event)
{
	QPainter paint(this);
	QPixmap pix;

	switch (_viewMode)
	{
	case unitScale:
		pix = _pict;
		break;
	case fitScale:
		pix = _pict.scaledToWidth(frameSize().width(),Qt::SmoothTransformation);
		pix = pix.scaledToWidth(frameSize().height(),Qt::SmoothTransformation);
		break;
	case otherScale:

		break;
	}
	
	_widthPix = pix.width();
	_heightPix = pix.height();

	if (pix.hasAlphaChannel())
	{
		QRect rect(frameGeometry());
		paint.fillRect(rect,_brushCanvas);
	}

	paint.scale(_currentScale, _currentScale);
	_pointPicture.setX(width()/2 - _widthPix/2);
	_pointPicture.setY(height()/2 - _heightPix/2);
	paint.drawPixmap(_pointPicture, pix);
	
}

void PhotoWidget::mouseMoveEvent(QMouseEvent *event)
{
	int dx = event->x() - _pointMouseX;
	int dy = event->y() - _pointMouseY;
	_pointPicture.setX(_pointPictureX + dx);
	_pointPicture.setY(_pointPictureY + dy);
	update();
}

void PhotoWidget::mousePressEvent(QMouseEvent *event)
{
	_pointMouseX = event->x();
	_pointMouseY = event->y();
	_pointPictureX = _pointPicture.x();
	_pointPictureY = _pointPicture.y();
}

void PhotoWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
	_pointPicture.setX(width()/2 - _widthPix/2);
	_pointPicture.setY(height()/2 - _heightPix/2);
	update();
}

void PhotoWidget::wheelEvent(QWheelEvent *event)
{
	float temp = _currentScale;
	_currentScale =_currentScale + event->delta() / 120;
	update();

	if (_currentScale < 1)
	{
		_currentScale = temp;
	}
}

void PhotoWidget::resizeEvent (QResizeEvent *event)
{
	update();
}