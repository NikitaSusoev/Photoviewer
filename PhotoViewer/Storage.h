#ifndef STORAGE_H
#define STORAGE_H

#include <QString>
#include <QSize>
#include <QList>
#include <QFile>

#include "model.h"

class Storage : public QObject
{
	Q_OBJECT
public:	
	struct Picture{
		QString filename;
		QPixmap pixmap;
		int frameIndex;
		QSize size;
		QString typeOfFile;
		Picture(){frameIndex = 0;}
	};

	QList <Picture> _cashPictures;

private:
	static Storage* singleStorage;
	Storage();

public:
	~Storage();

	QPixmap getPicture(Model::Element element, int width, int height);
	int getCountFramesFromWebP(QByteArray byteArray);
	static Storage *get();
};

#endif