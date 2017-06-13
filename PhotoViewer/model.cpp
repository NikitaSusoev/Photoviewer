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

		if (f1.open(QIODevice::ReadOnly))
		{
			element.filename = filename;

			if (filename.endsWith(".webp", Qt::CaseInsensitive))
			{
				int countFrames = Storage::get()->getCountFramesFromWebP(f1.readAll());

				for (int i = 0; i < countFrames; i++)
				{
					element.frameIndex = i;
					elements.append(element);
				}
			}
			else
			{
				element.frameIndex = 0;
				elements.append(element);
			}
		}
	}
	return elements;
}


QPixmap Model::getPixmapFromElement(Element element, int width, int height)
{
	return Storage::get()->getPicture(element, width, height);
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

void Model::deleteElements(QList <int> indexesElements)
{
	foreach(int index, indexesElements)
	{
		_elements.removeAt(indexesElements.first());
		_selections.removeAt(indexesElements.first());
	}

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

