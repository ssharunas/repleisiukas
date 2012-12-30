#include <QtGui/QApplication>
#include "mainwindow.h"

#include "model/filesystem/filesystemfactory.h"
#include "model/filesystem/local/localfilesystemhandler.h"

int main(int argc, char *argv[])
{
	FileSystemFactory::SetDefaultScheme("file");
	FileSystemFactory::RegisterFileSystemHandler("file", new LocalFileSystemHandler);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
