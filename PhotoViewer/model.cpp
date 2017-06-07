#include "model.h"
#include "Storage.h"
#include "photoviewer.h"
#include "photowidget.h"
#include "frameswidget.h"
#include <QList>
#include <QFile>
#include <QPixmap>

#include "webp/decode.h"
#include "webp/encode.h"
#include "webp/types.h"
#include "webp/mux.h"
#include "webp/demux.h"
#include "webp/mux_types.h"

Model::Model()
	: QObject()
{
}

Model::~Model()
{
}

void Model::addElement(QList <Element> elements)
{
	foreach(Element element, elements)
	{
		_elements.append(element);
		_selections.append(false);
	}	

	emit modelChanged();
	//return _lst.indexOf(str);
}

void Model::addElement(QStringList lst)
{

}

QString Model::getTypeOfFile(QString filename)
{
	if (filename.endsWith(".webp",Qt::CaseInsensitive))
	{
		return "WebP";
	}
	if (filename.endsWith(".jpeg",Qt::CaseInsensitive))
	{
		return "JPEG";
	}
	if (filename.endsWith(".jpg",Qt::CaseInsensitive))
	{
		return "JPG";
	}
	if (filename.endsWith(".png",Qt::CaseInsensitive))
	{
		return "PNG";
	}

	return "Unknown type";
}

void Model::removeSelection()
{
	for (int index = 0; index < _selections.count(); index++)
	{
		_selections.replace(index, false);
	}

	emit modelChanged();
}

QList <Model::Element> Model::generateAllFramesFromFilenames(QStringList lst)
{
	QList <Model::Element> elements;

	foreach(QString filename, lst){

		QFile f1(filename);
		Model::Element element;

		element.typeOfFile = getTypeOfFile(filename);

		if (filename.endsWith(".webp", Qt::CaseInsensitive))
		{
			if (f1.open(QIODevice::ReadOnly))
			{
				QByteArray ar = f1.readAll();
				WebPData webPData;
				webPData.bytes = (const uint8_t *)ar.constData();
				webPData.size = ar.size();
				WebPDemuxer *demux = WebPDemux(&webPData);
				int countFrames = WebPDemuxGetI(demux, WEBP_FF_FRAME_COUNT);

				element.filename = filename;

				for (int i = 0; i < countFrames; i++)
				{
					element.frameIndex = i;
					elements.append(element);
				}

			/*	if (countFrames > 1)
				{
					element.filename = filename;

					for (int i = 0; i < countFrames; i++)
					{	
						element.frameIndex = i;
						elements.append(element);
					}
				} 
				else
				{
					element.filename = filename;
					element.frameIndex = 0;
					elements.append(element);
				}*/
			}
		}
		else
		{
			element.filename = filename;
			element.frameIndex = 0;
			elements.append(element);
		}

	}
	return elements;
}


QPixmap Model::getPixmapFromElement(Element element, int width, int height)
{
	return Storage::get()->getPicture(element, width, height);
	/*QFile f1(element.filename);
	int w,h;
	QPixmap pix;

	if (f1.open(QIODevice::ReadOnly))
	{
		QByteArray ar = f1.readAll();

		if (element.filename.endsWith(".webp", Qt::CaseInsensitive))
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
				QImage img(pData1, w, h, QImage::Format_ARGB32);
				pix = QPixmap::fromImage(img);
			} 
			else
			{
				uint8_t *pData = WebPDecodeBGRA((const uint8_t *)ar.constData(), ar.size(), &w, &h);
				QImage img(pData, w, h, QImage::Format_ARGB32);
				pix = QPixmap::fromImage(img);
			}
		}
		else
		{
			QImage img(element.filename);
			pix = QPixmap::fromImage(img);
		}

		f1.close();

		if (width == 0 && height == 0)
		{
			return pix;
		}

		return pix.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	}*/
}

void Model::insertElement(QList <Element> elements, int after)
{
	foreach(Element element, elements)
	{
		_elements.insert(after, element);
		_selections.insert(after,false);
		after++;
	}

	emit modelChanged();
}

void Model::deleteElement(int index)
{
	_elements.removeAt(index);
	_selections.removeAt(index);
	emit modelChanged();
}

void Model::moveElement(QList <int> indexes, int newIndex)
{
	int oldIndex, toIndex;

	if (indexes.isEmpty())
	{
		return;
	}

	if (indexes.first() > newIndex)
	{
		oldIndex = indexes.last();
		toIndex = newIndex;
	} 
	else
	{	
		oldIndex = indexes.first();
		toIndex = newIndex - 1;
	}

	foreach (int index, indexes){

		setSelection(index, false);
		_elements.move(oldIndex, toIndex);
	}
	
	emit modelChanged();
}

bool Model::ifOnlyFirstIsSelected()
{
	return _selections.first();
}

bool Model::ifOnlyLastIsSelected()
{
	return _selections.last();
}

void Model::setSelection(int index, bool value)
{
	_selections.replace(index, value);
	emit modelChanged();
}

QList<int> Model::indexesSelectedElements()
{
	QList<int> trueSelections;
	int indexSelection = 0;

	foreach(bool selection, _selections)
	{
		
		if (selection)
		{
			int t = _selections.indexOf(selection);
			trueSelections.append(indexSelection);
		}

		indexSelection++;
	}

	return trueSelections;
}

QList <Model::Element> Model::selectedElements()
{
	QList<Element> SelectedElements;
	foreach(int indexSelectedElement, indexesSelectedElements()){

		SelectedElements.append(_elements.at(indexSelectedElement));
	}
/*
	if (selectedObjects().count() == 1)
	{
		return _elements.at(selectedObjects().first());
	}

	Model::Element element;
	element.filename = QString();*/

	return SelectedElements;
}
/*
int Model::indexSelectedFilename()
{
	if (indexesSelectedElements().count() == 1)
	{
		return indexesSelectedElements().first();
	}
	//return -1;
}*/

bool Model::isSelected(int index)
{
	if (indexesSelectedElements().contains(index))
	{
		return true;
	}

	return false;
}

Model *Model::singleModel = 0;

void Model::setElements(QList <Element> elements)
{
	_elements = elements;

	foreach(Element element, elements){

	}

	_selections.clear();

	for (int i = 0; i < _elements.count(); i++)
	{
		_selections.append(false);
	}
	_selections.replace(0,true);

	emit modelChanged();
}

Model * Model::get()
{
	if (singleModel == 0)
	{
		singleModel = new Model();
	}

	return singleModel;
}

