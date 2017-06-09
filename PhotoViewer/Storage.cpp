#include "Storage.h"
#include "Loader.h"


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
	int t = _cashPictures.count();
	QPixmap pixmap = Loader::get()->loadPixmapFromElement(element);
	Picture picture;
	picture.filename = element.filename;
	picture.frameIndex = element.frameIndex;

	if (width == 0 && height == 0)
	{
		picture.pixmap = pixmap;
	}else{
		picture.pixmap = pixmap.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	}
	
	picture.size.setWidth(width);
	picture.size.setHeight(height);
	_cashPictures.append(picture);

	return picture.pixmap;
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

