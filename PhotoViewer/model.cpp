#include "model.h"
#include "photoviewer.h"
#include "photowidget.h"
#include "frameswidget.h"
#include <QList>

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

void Model::moveElement(int oldIndex, int newIndex)
{
	_elements.move(oldIndex, newIndex);
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

QList<int> Model::selectedObjects()
{
	QList<int> trueSelections;

	foreach(bool selection, _selections)
	{
		if (selection)
		{
			trueSelections.append(_selections.indexOf(selection));
		}
	}

	return trueSelections;
}

QString Model::selectedFilename()
{
	if (selectedObjects().count() == 1)
	{
		return _elements.at(selectedObjects().first());
	}
	return QString();
}

int Model::indexSelectedFilename()
{
	if (selectedObjects().count() == 1)
	{
		return selectedObjects().first();
	}
	//return -1;
}

bool Model::isSelected(int index)
{
	if (selectedObjects().contains(index))
	{
		return true;
	}

	return false;
}

Model *Model::singleModel = 0;

void Model::setElements(QList <Element> elements)
{
	_elements = elements;
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

