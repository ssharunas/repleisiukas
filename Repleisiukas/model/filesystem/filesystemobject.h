#ifndef FILESYSTEMOBJECT_H
#define FILESYSTEMOBJECT_H

#include "ifilesystemobject.h"

#include <QScriptEngine>

class FileSystemObject : public IFileSystemObject
{
	Q_OBJECT
	public:
		explicit FileSystemObject(QScriptEngine *engine);
		~FileSystemObject();

	signals:
		
	public slots:

	protected:
		QScriptValue permissions;
		QScriptValue isClosed;


};

#endif // FILESYSTEMOBJECT_H
