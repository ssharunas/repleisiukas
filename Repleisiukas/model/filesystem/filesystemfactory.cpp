#include "filesystemfactory.h"

#include <QDebug>
#include <QHash>

namespace FileSystemFactory
{
	namespace
	{
		QString _defaultScheme;
		QHash<QString, IFilesystemHandler*> _handlers;
	}

	QString defaultScheme()
	{
		return _defaultScheme;
	}

	void setDefaultScheme(QString defaultScheme)
	{
		_defaultScheme = defaultScheme;
	}

	void registerFileSystemHandler(QString scheme, IFilesystemHandler* handler)
	{
		if(_handlers.contains(scheme))
			qWarning() << QString("Scheme '%1' is already registered. It will be replaced.").arg(scheme);

		_handlers[scheme] = handler;
	}

	IFilesystemHandler* getHandler(QString scheme)
	{
		if(scheme.isEmpty())
			scheme = defaultScheme();

		if(_handlers.contains(scheme))
			return _handlers[scheme];

		return 0;
	}

	QList<QString> registeredSchemes(){
		return _handlers.keys();
	}
}
