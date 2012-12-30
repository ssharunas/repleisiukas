#include "filesystemfactory.h"

#include <QDebug>
#include <QHash>

namespace FileSystemFactory
{
	namespace
	{
		QString _defaultScheme;
		QHash<QString, IFilesystemHandler*> handlers;
	}

	QString GetDefaultScheme()
	{
		return _defaultScheme;
	}

	void SetDefaultScheme(QString defaultScheme)
	{
		_defaultScheme = defaultScheme;
	}

	void RegisterFileSystemHandler(QString scheme, IFilesystemHandler* handler)
	{
		if(handlers.contains(scheme))
			qWarning() << QString("Scheme '%1' is already registered. It will be replaced.").arg(scheme);

		handlers[scheme] = handler;
	}

	IFilesystemHandler* GetHandler(QString scheme)
	{
		if(scheme.isEmpty())
			scheme = GetDefaultScheme();

		if(handlers.contains(scheme))
			return handlers[scheme];

		return 0;
	}

	QList<QString> registeredSchemes(){
		return handlers.keys();
	}
}
