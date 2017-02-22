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
}

PhotoWidget::~PhotoWidget()
{
	
}

void PhotoWidget::setPixmap(const QPixmap &pxm)
{
	_tempPix = pxm;
	
	_pict = pxm.scaledToWidth(frameSize().width());
	_pict = _pict.scaledToHeight(frameSize().height());
	
	_widthPix = _pict.width();
	_heightPix = _pict.height();
	update();
}

void PhotoWidget::showPicture(QString filename)
{
	setWindowTitle(filename);

	QFile file(filename);
	int wid, heig;

	if (file.open(QIODevice::ReadOnly))
	{
		QByteArray ar = file.readAll();
		uint8_t *pData = WebPDecodeBGRA((const uint8_t *)ar.constData(), ar.size(), &wid, &heig);
		QImage img(pData, wid, heig, QImage::Format_ARGB32);
		QPixmap pix = QPixmap::fromImage(img);
		setPixmap(pix);
		file.close();
	}
}

void PhotoWidget::modelChanged()
{
	showPicture(Model::get()->selectedFilename());
}

void PhotoWidget::paintEvent(QPaintEvent *event)
{
	QPainter paint(this);
	paint.scale(_currentScale, _currentScale);
	_pointPicture.setX(width()/2 - _widthPix/2);
	_pointPicture.setY(height()/2 - _heightPix/2);
	paint.drawPixmap(_pointPicture, _pict);
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
	if (!_tempPix.isNull())
	{
		setPixmap(_tempPix);
	}
	
}