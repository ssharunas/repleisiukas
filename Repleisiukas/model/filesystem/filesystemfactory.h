#ifndef FILESYSTEMFACTORY_H
#define FILESYSTEMFACTORY_H

#include <QString>
#include "ifilesystemhandler.h"

namespace FileSystemFactory
{
	QString GetDefaulktNamespace();
	void GetDefaulktNamespace(QString defaultNamespace);

	void RegisterFileSystemHandler(QString scheme, IFilesystemHandler* handler);
	IFilesystemHandler* GetHandler(QString scheme);
}

#endif // FILESYSTEMFACTORY_H
