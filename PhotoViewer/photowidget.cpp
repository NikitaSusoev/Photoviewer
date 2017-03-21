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
			  
		break;

	case fitScale:

		break;

	case otherScale:

		break;
	}


void PhotoWidget::setPixmap(const QPixmap &pxm)
{
	_pict = pxm;
	update();
}

void PhotoWidget::showPicture(Model::Element element)
{
	setWindowTitle(element.filename);
/*
	QFile file(element.filename);
	int wid, heig;

	if (file.open(QIODevice::ReadOnly))
	{
		QByteArray ar = file.readAll();
		uint8_t *pData = WebPDecodeBGRA((const uint8_t *)ar.constData(), ar.size(), &wid, &heig);
		QImage img(pData, wid, heig, QImage::Format_ARGB32);
		QPixmap pix = QPixmap::fromImage(img);*/
		setPixmap(Model::get()->getPixmapFromElement(element));
	//	file.close();
	//}
}


void PhotoWidget::modelChanged()
{
	if (!(Model::get()->selectedElement().filename == QString()))
	{
		showPicture(Model::get()->selectedElement());
	}	
}

void PhotoWidget::paintEvent(QPaintEvent *event)
{
	QPainter paint(this);
	
	QPixmap pix;
	pix = _pict.scaledToWidth(frameSize().width(),Qt::SmoothTransformation);
	pix = pix.scaledToWidth(frameSize().height(),Qt::SmoothTransformation);
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
	//if (!_tempPix.isNull())
	//{
	//	setPixmap(_tempPix);
	//}
	update();
}