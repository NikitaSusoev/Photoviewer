#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QList>
#include <QStringList>
#include <QPixmap>



class Model : public QObject
{
	Q_OBJECT
public:
	struct Element{
		QString filename; 
		int frameIndex;
		QString typeOfFile;
		Element(){frameIndex = 0;}
	};
private:
		QList <Element> _elements;
		QList <bool> _selections;

	static Model *singleModel;

	Model();

public:
	
	~Model();

	void addElement(QList <Element> elements);
	void insertElement(QList <Element> elements, int after);
	void deleteElement(int index);
	void moveElement(int oldIndex, int newIndex);
	bool ifOnlyFirstIsSelected();
	bool ifOnlyLastIsSelected();
	void setSelection(int index, bool value);
	QList<int> selectedObjects();
	Element selectedElement();
	int indexSelectedFilename();
	bool isSelected(int index);
	void addElement(QStringList lst);
	QString getTypeOfFile(QString filename);

	QList <Model::Element> generateAllFramesFromFilenames(QStringList lst);
	QPixmap getPixmapFromElement(Element);

	QList <Element> elements(){return _elements;}
	void setElements(QList <Element> elements);

	//bool isSelected(){return _selection;}

	static Model *get();


private:
	
signals:
	void modelChanged();
};

#endif // MODEL_H
