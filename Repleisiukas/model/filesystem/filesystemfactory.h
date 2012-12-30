#ifndef FILESYSTEMFACTORY_H
#define FILESYSTEMFACTORY_H

#include <QString>
#include <QList>
#include "ifilesystemhandler.h"

namespace FileSystemFactory
{
	QString GetDefaultScheme();
	void SetDefaultScheme(QString defaultScheme);

	void RegisterFileSystemHandler(QString scheme, IFilesystemHandler* handler);
	IFilesystemHandler* GetHandler(QString scheme);
    QList<QString> registeredSchemes();
}

#endif // FILESYSTEMFACTORY_H
