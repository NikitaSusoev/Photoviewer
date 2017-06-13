#include "Storage.h"
#include "Loader.h"
#include <QFile>

Storage::Storage(void)
{
}


QPixmap Storage::getPicture(Model::Element element, int width, int height)
{
	foreach(Picture picture, _cashPictures){

		if (picture.filename == element.filename && picture.frameIndex == element.frameIndex && picture.size.width() == width && picture.size.height() == height)
		{
			return picture.pixmap;
		}
	}

	Picture picture;
	picture.pixmap = Loader::get()->loadPixmapFromElement(element, width, height);
	picture.filename = element.filename;
	picture.frameIndex = element.frameIndex;
	picture.size.setWidth(width);
	picture.size.setHeight(height);
	_cashPictures.append(picture);

	return picture.pixmap;
}

int Storage::getCountFramesFromWebP(QByteArray byteArray)
{
	return Loader::get()->getCountFrame(byteArray);
}

Storage *Storage::singleStorage = 0;

Storage * Storage::get()
{
	if (singleStorage == 0)
	{
		singleStorage = new Storage();
	}

	return singleStorage;
}

Storage::~Storage(void)
{
}

