#ifndef LOADER_H
#define LOADER_H

#include <QPixmap>
#include <QFile>
#include "model.h"

class Loader : public QObject
{
	Q_OBJECT
public:
	

private:

	static Loader *singleLoader;
	Loader();

public:
	~Loader();

	static Loader *get();
	int getCountFrame(QByteArray byteArray);
	QPixmap loadPixmapFromElement(Model::Element element, int width, int height);
};

#endif

