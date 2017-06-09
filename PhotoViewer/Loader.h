#ifndef LOADER_H
#define LOADER_H

#include <QPixmap>
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

	QPixmap loadPixmapFromElement(Model::Element element);
};

#endif

