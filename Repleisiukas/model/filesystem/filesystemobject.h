#ifndef FILESYSTEMOBJECT_H
#define FILESYSTEMOBJECT_H

#include "ifilesystemobject.h"

#include <QScriptEngine>
#include <QtScript/QScriptClass>

class FileSystemObject : public QObject, public QScriptClass, IFileSystemObject
{
		Q_OBJECT
	public:
		explicit FileSystemObject(QScriptEngine *engine);
		~FileSystemObject();

	signals:
		
	public slots:

	protected:
		QScriptString name;
		QScriptValue permissions;
		QScriptValue isClosed;

		QScriptValue proto;
		QScriptValue ctor;

};

#endif // FILESYSTEMOBJECT_H
