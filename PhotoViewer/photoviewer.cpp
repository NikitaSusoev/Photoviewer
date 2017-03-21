#include "photoviewer.h"
#include <QFileDialog>
#include <QByteArray>
#include <QDir>
#include <QTreeWidgetItem>
#include <QTreeWidget>
#include <QFile>
#include "ImageNode.h"
#include "SpaceNode.h"
#include "model.h"

#include "webp/decode.h"
#include "webp/encode.h"
#include "webp/types.h"
#include "webp/mux.h"
#include "webp/demux.h"
#include "webp/mux_types.h"

PhotoViewer::PhotoViewer(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	 _currentNumber = 0;

	ui.setupUi(this);
	ui.back->setDisabled(true);
	ui.forward->setDisabled(true);

	connect(ui.widget, SIGNAL(selectionChanged()), this, SLOT(selectionChanged()));
	connect(Model::get(), SIGNAL(modelChanged()), this, SLOT(modelChanged()));
}

PhotoViewer::~PhotoViewer()
{

}

QString PhotoViewer::getSizePicture(QString filename)
{
	QFile file(filename);
	int wid, heig;

	if (file.open(QIODevice::ReadOnly))
	{
		QByteArray ar = file.readAll();
		WebPGetInfo((const uint8_t *)ar.constData(), ar.size(), &wid, &heig);
		file.close();
	}

	return (QString::number(wid) + " x " + QString::number(heig));
}

void PhotoViewer::createProperties(QString filename)
{
	if (filename == QString())
	{
		setProperties(QString(), QString(), QString(), QString());
	}
	else
	{
		setProperties(filename, getSizePicture(filename), QString::number(Model::get()->indexSelectedFilename() + 1), QString());
	}	
}

void PhotoViewer::setProperties(QString filename, QString size, QString numberOfPicture, QString others)
{
	ui.treeWidget->topLevelItem(0)->setText(1,filename);
	ui.treeWidget->topLevelItem(1)->setText(1,size);
	ui.treeWidget->topLevelItem(2)->setText(1,others);
	ui.treeWidget->topLevelItem(3)->setText(1,others);
	ui.treeWidget->topLevelItem(4)->setText(1,numberOfPicture);
	ui.treeWidget->topLevelItem(5)->setText(1,others);
	ui.treeWidget->topLevelItem(6)->setText(1,others);
	ui.treeWidget->topLevelItem(7)->setText(1,others);
}

void PhotoViewer::makeOwnScale()
{
	ui.pic->setScaleMode(unitScale);
}

void PhotoViewer::showAll()
{
	ui.pic->setScaleMode(fitScale);
}

void PhotoViewer::chooseFiles()
{

	QStringList lst = QFileDialog::getOpenFileNames(this,"Choose Files");
	
	if (lst.isEmpty())
		return;

	ui.statusBar->showMessage(QString::number(Model::get()->generateAllFramesFromFilenames(lst).count()));
	Model::get()->setElements(Model::get()->generateAllFramesFromFilenames(lst));
}


void PhotoViewer::backPicture()
{
	Model::get()->setSelection(_currentNumber,false);
	Model::get()->setSelection(_currentNumber - 1,true);
}

void PhotoViewer::forwardPicture()
{
	Model::get()->setSelection(_currentNumber,false);
	Model::get()->setSelection(_currentNumber + 1,true);
}

void PhotoViewer::modelChanged()
{
	createProperties(Model::get()->selectedElement().filename);

	if (!(Model::get()->selectedElement().filename == QString()))
	{
		ui.back->setDisabled(Model::get()->ifOnlyFirstIsSelected());
		ui.forward->setDisabled(Model::get()->ifOnlyLastIsSelected());
		_currentNumber = Model::get()->indexSelectedFilename();
	}
	else
	{
		ui.back->setDisabled(true);
		ui.forward->setDisabled(true);
	}
}

void PhotoViewer::addPictures()
{
	QStringList picturesLst = QFileDialog::getOpenFileNames(this,"Choose Files");

	if (picturesLst.isEmpty())
	{
		return;
	}

	Model::get()->addElement(Model::get()->generateAllFramesFromFilenames(picturesLst));
}

void PhotoViewer::save()
{
	QList <QImage *> images;
	
	QList <Model::Element> elements = Model::get()->elements();
	QString filename = QFileDialog::getSaveFileName(this, "Choose Folder", "", ".webp");

	if (filename.isEmpty())
	{
		return;
	}

	foreach( Model::Element element, elements){

		/*if (element.filename.contains(".webp",Qt::CaseInsensitive))
		{
			//должна быть проверка на несколько вебп в одном файле и вставка элементов, если их несколько 
		} 
		else
		{
		}*/
		QImage *image = new QImage(Model::get()->getPixmapFromElement(element).toImage());
		images.append(image);
	}

	QFile f(filename);
	f.open(QIODevice::WriteOnly);
	WebPData webPData = createWebPAnimation(images);

	foreach(QImage *image, images){
		delete image;
	}

	f.write((const char*)webPData.bytes,webPData.size);
	f.close();
}

QImage* PhotoViewer::getFrameFromFile(QString file)
{
	QImage *frame;
	FileData filedata = getFileData(file);

		if (file.contains(".webp",Qt::CaseInsensitive))
		{
			frame = new QImage(filedata.pData, filedata.width, filedata.height, QImage::Format_ARGB32);
		} 
		else
		{
			frame = new QImage(file);
		}

	return frame;
}

PhotoViewer::FileData PhotoViewer::getFileData(QString file)
{
	int w,h;
	QFile f1(file);
	FileData fileData;

	if (f1.open(QIODevice::ReadOnly))
	{
		QByteArray ar = f1.readAll();
		fileData.pData = WebPDecodeBGRA((const uint8_t *)ar.constData(), ar.size(), &w, &h);
		fileData.width = w;
		fileData.height = h;
	}
	f1.close();

	return fileData;
}



WebPData PhotoViewer::createWebPAnimation(QList<QImage *> images)
{
	WebPMux *mux = WebPMuxNew();
	WebPData webPData;
	WebPMuxAnimParams params;

	if (images.count() < 2)
	{
		WebPData *webPDataForImage;
		webPDataForImage->size = getWebPDataFromImage(images.first()).size;
		webPDataForImage->bytes = getWebPDataFromImage(images.first()).bytes; 
		WebPMuxSetImage(mux,webPDataForImage,1);
	} 
	else
	{
		foreach(QImage *image, images){
			WebPMuxFrameInfo fr;			
			fr.bitstream.size = getWebPDataFromImage(image).size;
			fr.bitstream.bytes = getWebPDataFromImage(image).bytes;
			fr.id = WEBP_CHUNK_ANMF;
			fr.x_offset = 0;
			fr.y_offset = 0;
			fr.duration = 500;
			fr.dispose_method = WEBP_MUX_DISPOSE_BACKGROUND;
			fr.blend_method = WEBP_MUX_BLEND;
			int push = WebPMuxPushFrame(mux, &fr, 1);	
		}

		params.bgcolor = 0;
		params.loop_count = 0;
		WebPMuxSetAnimationParams(mux,&params);
	}
	WebPMuxAssemble(mux,&webPData);
	return webPData;
}

WebPData PhotoViewer::getWebPDataFromImage(QImage *image)
{
	uint8_t *output;
	WebPData webPDataForImage;
	QImage img;

	if (image->hasAlphaChannel())
	{
		img = image->convertToFormat(QImage::Format_ARGB32);
		webPDataForImage.size = WebPEncodeLosslessBGRA((uint8_t *)img.bits(),img.width(),img.height(), img.bytesPerLine(),&output);
		webPDataForImage.bytes = output; 
	}
	else
	{
		img = image->convertToFormat(QImage::Format_RGB888);
		webPDataForImage.size = WebPEncodeLosslessRGB((uint8_t *)img.bits(),img.width(),img.height(), img.bytesPerLine(),&output); 
		webPDataForImage.bytes = output; 
	}

	return webPDataForImage;
}


