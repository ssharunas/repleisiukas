#include "qfilesystemaccess.h"
#include "filesystem/filesystemfactory.h"

#include <QRegExp>
#include <QStringList>

QFileSystemAccess::QFileSystemAccess(QScriptEngine *engine) :
	QObject(0), QScriptClass(engine)
{

}

IFileSystemObject* QFileSystemAccess::open(QString path, QString mode)
{
	IFileSystemObject* result = 0;

	QString scheme = getScheme(path);
	IFilesystemHandler* handler = FileSystemFactory::GetHandler(scheme);

	if(handler)
	{
		result = handler->open(path, mode);

		handler = 0;
	}

	return result;
}

bool QFileSystemAccess::exists(QString path)
{
	bool result = false;

	QString scheme = getScheme(path);
	IFilesystemHandler* handler = FileSystemFactory::GetHandler(scheme);

	if(handler)
	{
		result = handler->exists(path);

		handler = 0;
	}

	return result;
}

QString QFileSystemAccess::getScheme(QString path)
{
	QString scheme;

	if(!path.isEmpty())
	{
		QRegExp reg("^\\w[\\w\\d\\+\\.-]*");
		reg.indexIn(path);
		QStringList list = reg.capturedTexts();

		if(list.length() > 0)
			scheme = list[0];
	}

	return scheme;
}
