#include "photoviewer.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	PhotoViewer w;
	w.show();
	return a.exec();
}
