#include "filesystemfactory.h"

#include <QDebug>
#include <QHash>

namespace FileSystemFactory
{
	namespace
	{
		QString defaultNamespace;
		QHash<QString, IFilesystemHandler*> handlers;
	}

	QString GetDefaulktNamespace()
	{
		return defaultNamespace;
	}

	void GetDefaulktNamespace(QString defaultNamespace)
	{
		defaultNamespace = defaultNamespace;
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
			scheme = GetDefaulktNamespace();

		if(handlers.contains(scheme))
			return handlers[scheme];

		return 0;
	}
}
