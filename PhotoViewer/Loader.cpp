#include "Loader.h"

#include "webp/decode.h"
#include "webp/encode.h"
#include "webp/types.h"
#include "webp/mux.h"
#include "webp/demux.h"
#include "webp/mux_types.h"

#include <QFile>

Loader::Loader(void)
{
}


QPixmap Loader::loadPixmapFromElement(Model::Element element)
{
	QFile f1(element.filename);
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
		return pix;
	}
}

Loader::~Loader(void)
{
}

Loader *Loader::singleLoader = 0;

Loader * Loader::get()
{
	if (singleLoader == 0)
	{
		singleLoader = new Loader();
	}

	return singleLoader;
}
