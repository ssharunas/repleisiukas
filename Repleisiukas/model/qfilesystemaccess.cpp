#include "qfilesystemaccess.h"
#include "filesystem/filesystemfactory.h"
#include "filesystem/openmodes.h"

#include <QScriptEngine>
#include <QDir>
#include <QDebug>

QFileSystemAccess::QFileSystemAccess(QScriptEngine *engine) :
	QObject(0)
{
	_engine = engine;
}

IFileSystemObject *QFileSystemAccess::open(QString path)
{
	return open(path, MODE_READ);
}

IFileSystemObject* QFileSystemAccess::open(QString path, QString mode)
{
	IFileSystemObject* result = 0;

#ifdef Q_WS_WIN
	if(path.indexOf(':') == 1 && path[0].isLetter()){
		qWarning() << "Detected path with scheme" << path[0] <<". Assuming drive letter and pathcing path with file scheme.";
		path = "file:///" + path;
	}
#endif

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
	}else{
		_engine->currentContext()->throwError(QString("No handler was found for scheme '%1'.").arg(url.scheme()));
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
	}else{
		_engine->currentContext()->throwError(QString("No handler was found for scheme '%1'.").arg(url.scheme()));
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
