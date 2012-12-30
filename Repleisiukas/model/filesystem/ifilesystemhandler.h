#ifndef IFILESYSTEMHANDLER_H
#define IFILESYSTEMHANDLER_H

#include "ifilesystemobject.h"
#include <QScriptEngine>
#include <QString>
#include <QUrl>

class IFilesystemHandler
{
	public:
		virtual ~IFilesystemHandler() {}
        virtual IFileSystemObject* open(QUrl url, QString mode, QScriptEngine *engine) = 0;
        virtual bool exists(QUrl url) = 0;
};

#endif // IFILESYSTEMHANDLER_H
