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
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <QListWidget>
#include <QMessageBox>
#include <QDropEvent>
#include <QUrl>
#include <QDebug>

#include "webp/decode.h"
#include "webp/encode.h"
#include "webp/types.h"
#include "webp/demux.h"
#include "webp/mux.h"
#include "webp/mux_types.h"

FramesWidget::FramesWidget(QWidget *parent)
	: QWidget(parent)
{
	_timerID = 0;
	_startValueScroll = 0;
	_countCall = 0;
	_imageHeight = 180;
	_spaceWidth = 20;
	_rubberBand = 0;
	_isStartTimer = false;

	QVBoxLayout *box = new QVBoxLayout;
	_horScroll = new QScrollBar;
	_horScroll->setOrientation(Qt::Horizontal);

	box->addStretch();
	box->addWidget(_horScroll);

	setLayout(box);
	show();
	
	setAcceptDrops(true);

	connect(_horScroll, SIGNAL(valueChanged(int)), this, SLOT(valueChangedHorScroll(int)));
	connect(Model::get(), SIGNAL(modelChanged()), this, SLOT(modelChanged()));
}

FramesWidget::~FramesWidget()
{
	
}

void FramesWidget::setNodes(QList<Node *> lst)
{
	int widthFrames = 0;

	foreach(Node *node, lst)
	{
		node->setPos(node->pos().x(),frameSize().height()/2 - node->size().height()/2);
		widthFrames = widthFrames + node->size().width();
	}

	_nodes = lst;
	Node *nodeLast = _nodes.last();
	Node *nodeFirst = _nodes.first();
	int scrollRange = widthFrames - width();
	//int scrollRange =(nodeLast->pos().x() + nodeLast->size().width()) - nodeFirst->pos().x() - width();
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
				//generateOffsetPicture(node);
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
	sNode->setStartPoint(QPoint(0, (frameSize().height() - _imageHeight) / 2));
	sNode->setSize(_spaceWidth,_imageHeight);
	sNode->setFilename("---");
	sNode->setDragging(false);
	nodes.append(sNode);

	foreach(Model::Element element, elements){
	
		QPixmap tempPixmap = getPixmapPicture(element,_imageHeight);
		int imageWidth=tempPixmap.width();

		ImageNode *iNode = new ImageNode();
		iNode->setIndexNode(indexNode);
		iNode->setPos(imageNodeX,0);
		iNode->setStartPoint(QPoint(imageNodeX, (frameSize().height() - _imageHeight) / 2));
		iNode->setSize(imageWidth,_imageHeight);
		iNode->setPixm(tempPixmap);
		iNode->setFilename(element.filename);
		iNode->setDragging(false);
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
		sNode->setStartPoint(QPoint(imageNodeX+imageWidth, (frameSize().height() - _imageHeight) / 2));
		sNode->setSize(_spaceWidth,_imageHeight);
		sNode->setFilename("---");
		sNode->setDragging(false);
		nodes.append(sNode);

		imageNodeX = imageNodeX + imageWidth + _spaceWidth;
	}

	setNodes(nodes);
}

QPixmap FramesWidget::getPixmapPicture(Model::Element element, int heig)
{
	QPixmap pxm;
	pxm = Model::get()->getPixmapFromElement(element, width(), heig);
	return pxm;
}
/*
void FramesWidget::generateOffsetPicture(Node *node)
{
	int step = 0;
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
*/
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
	QList <Model::Element> elements = Model::get()->elements();
	QList <Model::Element> selectedElements = Model::get()->selectedElements(); 

	if (elements.isEmpty())
	{
		setFilenames(elements);
		_currentElements = elements;
		return;
	}

	if (!(compareElementsList(elements)))
	{
		setFilenames(elements);
		_currentElements = elements;
	}

	if (selectedElements.count() > 0)
	{
		selectElements(selectedElements);	
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
		Model::Element elementCur = _currentElements.at(i);
		Model::Element elementMod = elements.at(i);
		int resultCompareStrings = QString::compare(elementCur.filename,elementMod.filename);

		if (!((resultCompareStrings == 0) && (_currentElements.count() == elements.count()) && (elementCur.frameIndex == elementMod.frameIndex)))
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
	_strartPointForSelection = event->pos();
	if (event->button() != Qt::LeftButton)
	{
		QWidget::mousePressEvent(event);
		return;
	}

	//if (Model::get()->elements().isEmpty())
	//{
	//	update();
	//	return;
	//}

	_origin = event->pos();
	_pointMouseX = event->pos().x();
	_pointMouseY = event->pos().y();
	bool contain = false;

	//QList <Node *> tempNodes = _nodes;

	foreach(Node *node, _nodes)
	{
		QRect rect(node->pos(),node->size());
		if (rect.contains(event->x(),event->y()) && node->filename() != "---")
		{
			if (!Model::get()->isSelected(node->indexNode()))
			{
				Model::get()->removeSelection();
				Model::get()->setSelection(node->indexNode(),true);
			}

			contain = true;
		}
	}

	if (!contain)
	{
		Model::get()->removeSelection();
		_rubberBand = new QRubberBand(QRubberBand::Rectangle,this);
		_rubberBand->setGeometry(QRect(_origin, QSize()));
		_rubberBand->show();
	}else{

		_rubberBand = 0;
	}

	QList <Node *> tempNodes = _nodes;

	foreach(Node *node, tempNodes)
	{
		if (Model::get()->isSelected(node->indexNode()) && node->filename() != "---")
		{
			QString str = node->filename();
			ImageNode *imageNode = static_cast<ImageNode *>(node);
			imageNode->setStartPoint(QPoint(imageNode->pos().x(), (frameSize().height()- _imageHeight)/2));
			_nodes.move(_nodes.indexOf(node), tempNodes.count() - 1);
		}	
	}

	

	update();
}

void FramesWidget::mouseMoveEvent(QMouseEvent *event)
{
	if (event->buttons().testFlag(Qt::RightButton))
	{
		QWidget::mouseMoveEvent(event);
		return;
	}

	if (_nodes.isEmpty())
	{
		return;
	}

	_posMoveEvent = event->pos();

	if (_rubberBand != 0)
	{
		QRect selectionRect(_origin,event->pos());

		int rightEdge = width() - 20;
		int leftEdge = 20;
		int dx;
			  
		
			if (event->pos().x() > rightEdge || event->pos().x() < leftEdge)
			{
				if (event->pos().x() > rightEdge)
				{
					dx = event->pos().x() - rightEdge;

					if (dx > _dx )
					{
						_horScroll->setValue(_horScroll->value() + 1);

						if (_horScroll->value() != _horScroll->maximum())
						{
							_origin.setX(_origin.x() - 1);
						}	

					}
				}

				if (event->pos().x() < leftEdge)
				{
					dx = event->pos().x();

					if (dx < _dx )
					{		
						_horScroll->setValue(_horScroll->value() - 1);

						if (_horScroll->value() != _horScroll->minimum())
						{
							_origin.setX(_origin.x() + 1);		
						}								
					}
				}

				_dx = dx;

				if (_timerID == 0)
				{
					_timerID = startTimer(100);
				}

			} 
			else
			{
				if (_timerID != 0)
				{
					killTimer(_timerID);
					_timerID = 0;
				}		
			}
					
			//_horScroll->setValue(dx1);

		_rubberBand->setGeometry(selectionRect.normalized());
		
		foreach(Node *node, _nodes)
		{
			QRect rectNode(node->pos(),node->size());

			if (node->filename() != "---")
			{
				if (!Model::get()->isSelected(node->indexNode()) && selectionRect.intersects(rectNode))
				{
					Model::get()->setSelection(node->indexNode(), true);	
				}

				if (Model::get()->isSelected(node->indexNode()) && !selectionRect.intersects(rectNode))
				{
					Model::get()->setSelection(node->indexNode(), false);
				}
			}	
		}
	} 
	else
	{
		
		int dx;
		int dy;

		//dx = event->x() - _pointMouseX;
		//dy = event->y() - _pointMouseY;
		dx = _pointMouseX;
		dy = _pointMouseY;
		int i = 0;
		QPoint pointForMovePicture;
		pointForMovePicture = _strartPointForSelection - event->pos();

		if (pointForMovePicture.manhattanLength() > 5)
		{
			foreach(Node *node, _nodes)
			{
				QString str = node->filename();
				if (Model::get()->isSelected(node->indexNode()) && node->filename() != "---")
				{
					node->setDragging(true);
					_pointMouseX = event->x();
					_pointMouseY = event->y();
					//node->setPos(node->pos().x() + dx, node->pos().y() + dy);
					node->setPos(dx + i, dy + i);
					i = i + 3;
				}
			}
		}	
	}
	
	update();
}

void FramesWidget::mouseReleaseEvent ( QMouseEvent * event )
{

	if (_strartPointForSelection == event->pos() && event->button() != Qt::RightButton)
	{
		foreach (Node *node, _nodes){
			QRect rect(node->pos(),node->size());
			if (rect.contains(event->x(),event->y())){
				Model::get()->removeSelection();
				Model::get()->setSelection(node->indexNode(),true);
			}
		}
	}

	foreach(Node *node, _nodes)
	{
		QRect rect(node->pos(),node->size());

		if (rect.contains(event->x(),event->y()) && node->filename() == "---" && _strartPointForSelection == event->pos() && event->button() != Qt::RightButton)
		{
			Model::get()->removeSelection();
		}
	}

	if (event->button() != Qt::LeftButton)
	{
		QWidget::mouseReleaseEvent(event);
		return;
	}

	if (_nodes.isEmpty())
	{
		return;
	}

	if (_rubberBand != 0)
	{
		delete _rubberBand;
		_rubberBand = 0;

		if (_timerID != 0)
		{
			killTimer(_timerID);
			_timerID = 0;
		}
	} 
	else
	{
		QList <Node *> tempNodes = _nodes;
		bool isMoved = false;
		bool isSpace = false;
		int indexNode;

		foreach (Node *node, tempNodes){

			node->setDragging(false);
			QRect rect(node->pos(),node->size());

			if (rect.contains(event->x(),event->y()) && !Model::get()->isSelected(node->indexNode()))
			{
				isMoved = true;
				indexNode = node->indexNode();

				if (node->filename() == "---")
				{
					isSpace = true;
				}
			}
		}

		if (isMoved)
		{
			if (indexNode == _nodes.last()->indexNode() || isSpace)
			{
				Model::get()->moveElement(Model::get()->indexesSelectedElements(), indexNode);
			}
			else if (event->pos().x() - _strartPointForSelection.x() > 0)
			{
				Model::get()->moveElement(Model::get()->indexesSelectedElements(), indexNode + 1);
			} 
			else
			{
				Model::get()->moveElement(Model::get()->indexesSelectedElements(), indexNode);
			}
		}
		else
		{
			foreach (Node *node, tempNodes){

				if (Model::get()->isSelected(node->indexNode()))
				{
					//ImageNode *imageNode = static_cast<ImageNode *>(node);
					node->setPos(node->startPoint().x(), node->startPoint().y());
				}
			}
		}
	}

update();
}

void FramesWidget::resizeEvent(QResizeEvent *event)
{
	if (!_nodes.isEmpty())
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
	if (_rubberBand != 0)
	{
		delete _rubberBand;
		_rubberBand = 0;
	}
	
	bool contain = false;
	_contextPoint.setX(event->x());
	_contextPoint.setY(event->y());
	
	QMenu menu;
	
	foreach(Node *node, _nodes)
	{
		QRect rect(node->pos(),node->size());

		if (rect.contains(_contextPoint.x(),_contextPoint.y()) && !(node->filename() == "---"))
		{
			menu.addAction("Delete", this, SLOT(deletePictures()));
			contain = true;
		}

		if (rect.contains(_contextPoint.x(),_contextPoint.y()) && (node->filename() == "---"))
		{
			menu.addAction("Add", this, SLOT(insertPictures()));
			contain = true;
		}
	}

	if (contain)
	{
		menu.exec(event->globalPos());
	}
	
}

void FramesWidget::timerEvent(QTimerEvent *event)
{
	_horScroll->setValue(_horScroll->value() + (_dx * width())/2000);

	if (_horScroll->value() != _horScroll->minimum() && _horScroll->value() != _horScroll->maximum())
	{
		_origin.setX(_origin.x() - (_dx * width())/2000);
	}
	
	QRect selectionRect(_origin, _posMoveEvent);
	_rubberBand->setGeometry(selectionRect.normalized());	
}

void FramesWidget::dragEnterEvent(QDragEnterEvent *event)
{
	event->accept();
}

void FramesWidget::dropEvent(QDropEvent *event)
{
	QList<QUrl> urls = event->mimeData()->urls();
	QStringList list;

	foreach(QUrl url, urls)
	{
		list.append(url.path().remove(0 , 1));
	}

	if (list.isEmpty())
	{
		return;
	}

	Model::get()->insertElement(Model::get()->generateAllFramesFromFilenames(list),1);
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
	Model::get()->deleteElements(Model::get()->indexesSelectedElements());
}
