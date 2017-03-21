#ifndef PHOTOVIEWER_H
#define PHOTOVIEWER_H

#include <QtGui/QMainWindow>
#include "ui_photoviewer.h"
#include "webp/mux_types.h"
#include "model.h"

class PhotoViewer : public QMainWindow
{
	Q_OBJECT

	QString _path;
	QStringList _lst;
	int _currentNumber;
	QString _filename;

public:
	PhotoViewer(QWidget *parent = 0, Qt::WFlags flags = 0);
	~PhotoViewer();

	struct FileData{
		uint8_t *pData; 
		int width;
		int height;
		FileData(){pData = 0;width = 0;height = 0;}
	};

	void showPicture(QString filename);
	QString getSizePicture(QString str);
	void createProperties(QString filename);
	void setProperties(QString filename, QString size, QString numberOfPicture, QString others);
	WebPData createWebPAnimation(QList<QImage *> images);
	WebPData getWebPDataFromImage(QImage *image);
	QImage* getFrameFromFile(QString file);
	FileData getFileData(QString file);
	

public slots:
	void chooseFiles();
	void backPicture();
	void forwardPicture();
	void modelChanged();
	void addPictures();
	void save();

protected:

private:
	Ui::PhotoViewerClass ui;
};

#endif // PHOTOVIEWER_H
