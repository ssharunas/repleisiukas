#ifndef IFILESYSTEMHANDLER_H
#define IFILESYSTEMHANDLER_H

#include "ifilesystemobject.h"
#include <QString>

class IFilesystemHandler
{
	public:
		virtual ~IFilesystemHandler() {}
		virtual IFileSystemObject* open(QString path, QString mode) = 0;
		virtual bool exists(QString path) = 0;
};

#endif // IFILESYSTEMHANDLER_H
