#include "localfilesystemhandler.h"
#include "localfolderobject.h"
#include <QDebug>
#include <QDir>
#include <QUrl>
#include <QFileInfo>

LocalFileSystemHandler::LocalFileSystemHandler(QObject *parent) :
	QObject(parent)
{
}

IFileSystemObject *LocalFileSystemHandler::open(QUrl url, QString mode, QScriptEngine *engine)
{
    QString path = url.path();
    if(path.startsWith("~")){
        if(path.length() <= 1 || path[1] == '/' || path[1] == QDir::separator()) {
            path.replace(0, 1, QDir::homePath());
            url.setPath(path);
        }
    }

    QFileInfo fileInfo (url.path());
    qDebug() << "file handler" << fileInfo.exists() << path;

    if(fileInfo.exists()){
        if(fileInfo.isDir()){
            qDebug() << "returning directory";
            return new LocalFolderObject(url, mode, engine);
        }
    }


	return 0;
}

bool LocalFileSystemHandler::exists(QUrl url)
{
	return QFile::exists(url.path());
}
