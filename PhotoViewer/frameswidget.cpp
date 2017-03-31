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
#include <QRubberBand>

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
	_imageHeight = 180;
	_spaceWidth = 20;

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

void FramesWidget::selectElements(QList <Model::Element> elements)
{
	foreach(Model::Element element, elements){

		foreach(Node *node, _nodes)
		{
			if ((node->filename() == element.filename) && (node->indexFrame() == element.frameIndex))
			{
				generateOffsetPicture(node);
			} 
		}
	}
	
	update();
}

void FramesWidget::setFilenames(QList <Model::Element> elements)
{
	_horScroll->setValue(0);
	QList<Node *> nodes;

	int imageNodeX = _spaceWidth;
	int indexNode = 0;

	SpaceNode *sNode = new SpaceNode();
	sNode->setIndexNode(indexNode);
	sNode->setPos(0,0);
	sNode->setSize(_spaceWidth,_imageHeight);
	sNode->setFilename("---");
	nodes.append(sNode);

	foreach(Model::Element element, elements){
	
		int imageWidth=getPixmapPicture(element,_imageHeight).width();

		ImageNode *iNode = new ImageNode();
		iNode->setIndexNode(indexNode);
		iNode->setPos(imageNodeX,0);
		iNode->setSize(imageWidth,_imageHeight);
		iNode->setPixm(getPixmapPicture(element,_imageHeight));
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
		sNode->setPos(imageNodeX+imageWidth,0);
		sNode->setSize(_spaceWidth,_imageHeight);
		sNode->setFilename("---");
		nodes.append(sNode);

		imageNodeX = imageNodeX + imageWidth + _spaceWidth;
	}

	setNodes(nodes);
}

QPixmap FramesWidget::getPixmapPicture(Model::Element element, int heig)
{
	QPixmap pxm;
	pxm = Model::get()->getPixmapFromElement(element).scaledToHeight(heig, Qt::SmoothTransformation);

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

	if (Model::get()->selectedElements().count() > 0)
	{
		selectElements(Model::get()->selectedElements());	
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
	_origin = event->pos();

	_rubberBand = new QRubberBand(QRubberBand::Rectangle,this);
	_rubberBand->setGeometry(QRect(_origin, QSize()));
	_rubberBand->show();


	if (!Model::get()->selectedElements().isEmpty())
	{
		foreach(int indexSelectedElement, Model::get()->indexesSelectedElements()){

			Model::get()->setSelection(indexSelectedElement, false);
		}
		//Model::get()->setSelection(Model::get()->indexSelectedFilename(), false);
	}
	
	foreach(Node *node, _nodes)
	{
		QRect rect(node->pos(),node->size());
		
		if ((rect.contains(event->x(),event->y())) && (!(node->filename() == "---")))
		{			
			Model::get()->setSelection(node->indexNode(), true);

			_startValueNodeX = node->pos().x();
			_pointMouseX = event->x();

			_nodes.move(_nodes.indexOf(node),_nodes.count() - 1);

			_rubberBand->hide();

			break;

		}else{
		}
	}

	update();
}


void FramesWidget::mouseMoveEvent(QMouseEvent *event)
{
	QRect selectionRect(_origin,event->pos());
	_rubberBand->setGeometry(selectionRect.normalized());

	if (!_rubberBand->isHidden())
	{
		foreach(Node *node, _nodes)
		{
			QRect rect(node->pos(),node->size());
			if (selectionRect.intersects(rect))
			{
				Model::get()->setSelection(node->indexNode(),true);
			}
		}
	} else {

		int dx;
		dx = event->x() - _pointMouseX;
		foreach(int indexSelectedElement, Model::get()->indexesSelectedElements()){

			foreach(Node *node, _nodes)
			{
				if (node->indexNode() == indexSelectedElement)
				{
					_pointMouseX = event->x();
					node->setPos(node->pos().x() + dx,node->pos().y());
				} 
			}
		}
	}
	
	update();
}

void FramesWidget::mouseReleaseEvent ( QMouseEvent * event )
{
	
	QRect selectionRect(_origin,event->pos());
	//QList <Model::Element> elements;
	if (_rubberBand->isHidden())
	{

		QList <Model::Element> elements;

		foreach(Node *node, _nodes)
		{
			QRect rect(node->pos(),node->size());

			if (selectionRect.intersects(rect))
			{
				Model::get()->setSelection(node->indexNode(),true);

			}

			foreach(int indexSelectedElement, Model::get()->indexesSelectedElements()){

				if (node->indexNode() == indexSelectedElement && (!(node->filename() == "---")))
				{
					if ((node->pos().x() - _startValueNodeX) > (node->size().width() + _spaceWidth)) 
					{
						int afterIndex = (node->pos().x() - _startValueNodeX)/(node->size().width() + _spaceWidth) + node->indexNode() + 1;
						elements.append(Model::get()->selectedElements().at(indexSelectedElement));//correct
						Model::get()->insertElement(elements,afterIndex);//correct
						Model::get()->deleteElement(node->indexNode());//correct

					} else if ((_startValueNodeX - node->pos().x()) > (node->size().width() + _spaceWidth))
					{
						int afterIndex = node->indexNode() - (_startValueNodeX - node->pos().x())/(node->size().width() + _spaceWidth);
						elements.append(Model::get()->selectedElements().first());//correct
						Model::get()->insertElement(elements,afterIndex);//correct
						Model::get()->deleteElement(node->indexNode() + elements.count());//correct

					}else{

						node->setPos(_startValueNodeX, node->pos().y());
					}

					update();
				} 
			}
		}

	} 
	else
	{
		_rubberBand->hide();
	}
		
	delete _rubberBand;
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
