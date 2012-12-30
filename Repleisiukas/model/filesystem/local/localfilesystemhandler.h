#ifndef LOCALFILESYSTEMHANDLER_H
#define LOCALFILESYSTEMHANDLER_H

#include "../ifilesystemhandler.h"

class LocalFileSystemHandler : public QObject, public IFilesystemHandler
{
		Q_OBJECT
	public:
		explicit LocalFileSystemHandler(QObject *parent = 0);
		
		virtual IFileSystemObject* open(QUrl url, QString mode, QScriptEngine *engine);
		virtual bool exists(QUrl url);
};

#endif // LOCALFILESYSTEMHANDLER_H
