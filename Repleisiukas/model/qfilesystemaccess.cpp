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
	initHelp();
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

QString QFileSystemAccess::help()
{
	return help(QString());
}

QString QFileSystemAccess::help(QString param)
{
	QString result = "Universal file system access functions. Allows access various files: local, network, virtual. Type of resource is determined by scheme. List of supported schemes can be retrieved using function schemes().\nExample of usage: FS.open('/home/user/file.txt', 'r').content\nFunctions:\n";

	if(param.isEmpty())
		result += getHelp(param);
	else
		result = getHelp(param);

	return result;
}

void QFileSystemAccess::initHelp()
{
	registerFunction("open", "Opens file for reading.", "File system object or null if resource does not exist",
					 QList<IHelpfull::HelphulParam>()
						<< HelphulParam("path", "url of resource")
					 << HelphulParam("mode", "access mode ['r', 'w', 'rw', 'a']"));

	registerFunction("open", "Opens file for reading.", "File system object or null if resource does not exist",
					 QList<IHelpfull::HelphulParam>() << HelphulParam("path", "url of resource"));

	registerFunction("exists", "Checks if resource does exist.", "true if resource exists. false otherwise.",
					 QList<IHelpfull::HelphulParam>() << HelphulParam("path", "url of resource"));

	registerFunction("schemes", "Returns a list of supported schemes", "", QList<IHelpfull::HelphulParam>());

	registerProperty("cwd", "Current working directory.", "path of application woring directory..");

}
