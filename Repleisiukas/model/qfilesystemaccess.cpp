#include "qfilesystemaccess.h"
#include "filesystem/filesystemfactory.h"

#include <QScriptEngine>
#include <QDir>
#include <QDebug>

QFileSystemAccess::QFileSystemAccess(QScriptEngine *engine) :
	QObject(0)
{
	_engine = engine;
}

IFileSystemObject* QFileSystemAccess::open(QString path, QString mode)
{
	IFileSystemObject* result = 0;

	QUrl url(path);

	qDebug()<< "path:" << url.path()
			<< "password:" << url.password()
			<< "scheme:" << url.scheme()
			<< "host:" << url.host()
			<< "local:" << url.isLocalFile()
			<< "relative:" << url.isRelative()
			<< "userInfo:" << url.userInfo()
			<< (url.isValid() ? "valid" : "invalid");

	IFilesystemHandler* handler = FileSystemFactory::GetHandler(url.scheme());

	if(handler)
	{
		result = handler->open(url, mode, _engine);

		handler = 0;
	}

	return result;
}

bool QFileSystemAccess::exists(QString path)
{
	bool result = false;

	QUrl url(path);
	IFilesystemHandler* handler = FileSystemFactory::GetHandler(url.scheme());

	if(handler)
	{
		result = handler->exists(url);

		handler = 0;
	}

	return result;
}

QString QFileSystemAccess::cwd()
{
	return QDir::currentPath();
}

QString QFileSystemAccess::toString()
{
	return QString("[File system access]");
}

QStringList QFileSystemAccess::schemes()
{
	return FileSystemFactory::registeredSchemes();
}
