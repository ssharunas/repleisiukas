#include <QApplication>

#include "ui/mainwindow.h"
#include "model/filesystem/filesystemfactory.h"
#include "model/filesystem/local/localfilesystemhandler.h"

int main(int argc, char *argv[])
{
	FileSystemFactory::setDefaultScheme("file");
	FileSystemFactory::registerFileSystemHandler("file", new LocalFileSystemHandler);

	QApplication a(argc, argv);

	MainWindow w;
	w.show();

	return a.exec();
}
