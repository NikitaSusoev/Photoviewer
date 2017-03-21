#include "frameswidget.h"
#include <QMouseEvent>
#include <QScrollBar>
#include <QLayout>
#include <QSpacerItem>
#include <QMenu>
#include "model.h"
#include "Node.h"
#include "SpaceNode.h"
#include "ImageNode.h"
#include <QFile>
#include <QFileDialog>

#include "webp/decode.h"
#include "webp/encode.h"
#include "webp/types.h"
#include "webp/demux.h"
#include "webp/mux.h"
#include "webp/mux_types.h"

FramesWidget::FramesWidget(QWidget *parent)
	: QWidget(parent)
{
	_startValueScroll = 0;
	_countCall = 0;

	QVBoxLayout *box = new QVBoxLayout;
	_horScroll = new QScrollBar;
	_horScroll->setOrientation(Qt::Horizontal);

	box->addStretch();
	box->addWidget(_horScroll);

	setLayout(box);
	show();
	
	connect(_horScroll, SIGNAL(valueChanged(int)), this, SLOT(valueChangedHorScroll(int)));
	connect(Model::get(), SIGNAL(modelChanged()), this, SLOT(modelChanged()));
}

FramesWidget::~FramesWidget()
{
	
}

void FramesWidget::setNodes(QList<Node *> lst)
{
	foreach(Node *node, lst)
	{
		node->setPos(node->pos().x(),frameSize().height()/2 - node->size().height()/2);
	}

	_nodes = lst;
	Node *node = _nodes.last();
	int scrollRange =(node->pos().x() + node->size().width()) - frameSize().width();
	//int offsetScrollByAdd = _horScroll->maximum() - scrollRange;
	_horScroll->setRange(0,scrollRange);
	//_horScroll->setValue(_horScroll->value() - offsetScrollByAdd);
	update();
}

void FramesWidget::setPixmap(QPixmap &pix)
{
	_pict = pix;
}

QString FramesWidget::getFilenameSelectedPicture()
{
	foreach(Node *node, _nodes)
	{
		if (node->isSelect())
		{
			return node->filename();
		}
	}
}

void FramesWidget::selectElement(Model::Element element)
{
	foreach(Node *node, _nodes)
	{
		if ((node->filename() == element.filename) && (node->indexFrame() == element.frameIndex))
		{
			generateOffsetPicture(node);
		} 
	}

	update();
}

void FramesWidget::setFilenames(QList <Model::Element> elements)
{
	_horScroll->setValue(0);
	QList<Node *> nodes;

	int iH = 180;
	int sW = 20;
	int iX = sW;
	int indexNode = 0;

	SpaceNode *sNode = new SpaceNode();
	sNode->setIndexNode(indexNode);
	sNode->setPos(0,0);
	sNode->setSize(sW,iH);
	sNode->setFilename("---");
	nodes.append(sNode);

	foreach(Model::Element element, elements){
	
		int iW=getPixmapPicture(element,iH).width();//getWidthPicture(str, iH);

		ImageNode *iNode = new ImageNode();
		iNode->setIndexNode(indexNode);
		iNode->setPos(iX,0);
		iNode->setSize(iW,iH);
		iNode->setPixm(getPixmapPicture(element,iH));
		iNode->setFilename(element.filename);
		iNode->setIndexFrame(element.frameIndex);
		iNode->setSelection(false);

		if ((elements.first().filename == element.filename) && (element.frameIndex == 0))
		{
			iNode->setSelection(true);
		}

		nodes.append(iNode);

		indexNode = indexNode +1;

		SpaceNode *sNode = new SpaceNode();
		sNode->setIndexNode(indexNode);
		sNode->setPos(iX+iW,0);
		sNode->setSize(sW,iH);
		sNode->setFilename("---");
		nodes.append(sNode);

		iX=iX+iW+sW;
	}

	setNodes(nodes);
}
/*
int FramesWidget::getWidthPicture(QString filename, int heig)
{
	QFile file(filename);
	int w,h,wid;

	if (file.open(QIODevice::ReadOnly))
	{
		QByteArray ar = file.readAll();
		uint8_t *pData = WebPDecodeBGRA((const uint8_t *)ar.constData(), ar.size(), &w, &h);
		QImage img(pData, w, h, QImage::Format_ARGB32);
		QPixmap pix = QPixmap::fromImage(img);
		wid = pix.scaledToHeight(heig).width();
		file.close();
	}

	return wid;
}
*/
QPixmap FramesWidget::getPixmapPicture(Model::Element element, int heig)
{
	//QFile f1(element.filename);
	//int w,h;
	QPixmap pxm;
	//QPixmap pix;
	/*
	if (f1.open(QIODevice::ReadOnly))
	{
		QByteArray ar = f1.readAll();

		if (element.filename.contains(".webp", Qt::CaseInsensitive))
		{
			WebPData webPData;
			webPData.bytes = (const uint8_t *)ar.constData();
			webPData.size = ar.size();
			WebPDemuxer *demux = WebPDemux(&webPData);
			int countFrames = WebPDemuxGetI(demux, WEBP_FF_FRAME_COUNT);

			if (countFrames > 1)
			{
				WebPIterator iter;
				WebPDemuxGetFrame(demux,element.frameIndex,&iter);
				uint8_t *pData1 = WebPDecodeBGRA(iter.fragment.bytes, iter.fragment.size, &w, &h);
				QImage img(pData1, w, h, QImage::Format_ARGB32);//QImage img(iter.fragment.bytes, iter.width, iter.height, QImage::Format_ARGB32);
				pix = QPixmap::fromImage(img);
				pxm = pix.scaledToHeight(heig);
			} 
			else
			{
				uint8_t *pData = WebPDecodeBGRA((const uint8_t *)ar.constData(), ar.size(), &w, &h);
				QImage img(pData, w, h, QImage::Format_ARGB32);
				pix = QPixmap::fromImage(img);
				pxm = pix.scaledToHeight(heig);
			}
		}
		else
		{
			QImage img(element.filename);
			pix = QPixmap::fromImage(img);
			pxm = pix.scaledToHeight(heig);
		}

		f1.close();
	}
	*/
	pxm = Model::get()->getPixmapFromElement(element).scaledToHeight(heig, Qt::SmoothTransformation);
	/*
	if (file.open(QIODevice::ReadOnly))
		{
			QByteArray ar = file.readAll();
			uint8_t *pData = WebPDecodeBGRA((const uint8_t *)ar.constData(), ar.size(), &w, &h);
			QImage img(pData, w, h, QImage::Format_ARGB32);
			QPixmap pix = QPixmap::fromImage(img);
			pxm = pix.scaledToHeight(heig);
			file.close();
		}
		*/
	return pxm;
}



void FramesWidget::generateOffsetPicture(Node *node)
{
	int step = 40;
	int offset = node->pos().x() + node->size().width() + step - width();

	if (offset >= 0)
	{
		_horScroll->setValue(offset + _horScroll->value()); 
	}
	else
	{
		if (node->pos().x() - step < 0)
		{
			_horScroll->setValue(_horScroll->value() + node->pos().x() - step);
		}
	}
}


void FramesWidget::valueChangedHorScroll(int value)
{
	foreach(Node *node, _nodes)
	{
		node->setPos(node->pos().x() - (value - _startValueScroll), node->pos().y());
	}

	_startValueScroll = value;
	update();
}

void FramesWidget::modelChanged()
{
	if (!(compareElementsList(Model::get()->elements())))
	{
		setFilenames(Model::get()->elements());
		_currentElements = Model::get()->elements();
	}

	if (!(Model::get()->selectedElement().filename == QString()))
	{
		selectElement(Model::get()->selectedElement());	
	}	
}

bool FramesWidget::compareElementsList(QList <Model::Element> elements)
{
	if (_currentElements.isEmpty())
	{
		return false;
	}

	for (int i = 0; i < elements.count(); i++)
	{
		if (!((QString::compare(_currentElements.at(i).filename,elements.at(i).filename) == 0) && (_currentElements.count() == elements.count())))
		{
			return false;
		}
	}

	return true;
}

void FramesWidget::paintEvent(QPaintEvent *event)
{
	QPainter paint(this);
	foreach(Node *node, _nodes)
	{
		node->paint(&paint, _pict);
	}
}

void FramesWidget::mousePressEvent(QMouseEvent *event)
{
	if (!(Model::get()->selectedElement().filename == QString()))
	{
		Model::get()->setSelection(Model::get()->indexSelectedFilename(), false);
	}
	
	foreach(Node *node, _nodes)
	{
		QRect rect(node->pos(),node->size());
		
		if ((rect.contains(event->x(),event->y())) && (!(node->filename() == "---")))
		{
			
			//_pointPictureX = node->pos().x();
			//_pointPictureY = node->pos().y();
			
			Model::get()->setSelection(node->indexNode(), true);

			_pointMouseX = event->x();
			//_pointMouseY = event->y();
			break;
		} 
	}

	update();
}


void FramesWidget::mouseMoveEvent(QMouseEvent *event)
{
	int dx;
	dx = event->x() - _pointMouseX;

	foreach(Node *node, _nodes)
	{
		if (node->indexNode() == Model::get()->indexSelectedFilename())
		{
			_pointMouseX = event->x();
			node->setPos(node->pos().x() + dx,node->pos().y());

		} 
	}


	//int dx = event->x() - _pointMouseX;
	//int dy = event->y() - _pointMouseY;
	//_pointPicture.setX(_pointPictureX + dx);
	//_pointPicture.setY(_pointPictureY + dy);
	update();
}

void FramesWidget::resizeEvent(QResizeEvent *event)
{
	if (!(_nodes.isEmpty()))
	{
		setNodes(_nodes);
	}
	
	update();
}

int FramesWidget::indexAfter()
{
	foreach(Node *node, _nodes)
	{
		QRect rect(node->pos(),node->size());

		if (rect.contains(_contextPoint.x(),_contextPoint.y()))
		{
			return (node->indexNode());
		} 
	}
}


void FramesWidget::contextMenuEvent(QContextMenuEvent *event)
{
	_contextPoint.setX(event->x());
	_contextPoint.setY(event->y());
	
	QMenu menu;
	menu.addAction("Add", this, SLOT(insertPictures()));
	foreach(Node *node, _nodes)
	{
		QRect rect(node->pos(),node->size());

		if (rect.contains(_contextPoint.x(),_contextPoint.y()) && !(node->filename() == "---"))
		{
			menu.addAction("Delete", this, SLOT(deletePictures()));
		} 
	}
	
	menu.exec(event->globalPos());
}



void FramesWidget::insertPictures()
{
	QStringList addLst = QFileDialog::getOpenFileNames(this,"Choose Files");

	if (addLst.isEmpty())
	{
		return;
	}

	Model::get()->insertElement(Model::get()->generateAllFramesFromFilenames(addLst),indexAfter());
}

void FramesWidget::deletePictures()
{
	Model::get()->deleteElement(indexAfter());
}
