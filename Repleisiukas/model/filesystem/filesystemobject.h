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

		static void registerMetaType(QScriptEngine *engine);

	signals:
		
	public slots:

	protected:
		QScriptValue _isClosed;


};

#endif // FILESYSTEMOBJECT_H
