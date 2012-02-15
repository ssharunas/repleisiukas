#ifndef LOCALFOLDEROBJECT_H
#define LOCALFOLDEROBJECT_H

#include "../filesystemobject.h"

#include <QScriptEngine>
#include <QtScript/QScriptClass>

class LocalFolderObject : public FileSystemObject
{
		Q_OBJECT
	public:
		LocalFolderObject(QScriptEngine *engine);
		~LocalFolderObject();

		
	signals:
		
	public slots:
		
};

#endif // LOCALFOLDEROBJECT_H
