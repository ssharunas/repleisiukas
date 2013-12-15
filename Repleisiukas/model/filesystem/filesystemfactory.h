#ifndef FILESYSTEMFACTORY_H
#define FILESYSTEMFACTORY_H

#include <QString>
#include <QList>
#include "ifilesystemhandler.h"

namespace FileSystemFactory
{
	QString defaultScheme();
	void setDefaultScheme(QString defaultScheme);

	void registerFileSystemHandler(QString scheme, IFilesystemHandler* handler);
	IFilesystemHandler* getHandler(QString scheme);
	QList<QString> registeredSchemes();
}

#endif // FILESYSTEMFACTORY_H
