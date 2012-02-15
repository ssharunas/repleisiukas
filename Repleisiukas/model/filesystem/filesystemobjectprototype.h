#ifndef FILESYSTEMOBJECTPROTOTYPE_H
#define FILESYSTEMOBJECTPROTOTYPE_H

#include "ifilesystemobject.h"

#include <QtCore/QObject>
#include <QtScript/QScriptable>
#include <QtScript/QScriptValue>

class FileSystemObjectPrototype :  public QObject, public QScriptable /*, public IFileSystemObject*/
{
	Q_OBJECT

	public:
		FileSystemObjectPrototype(QObject *parent = 0);
		~FileSystemObjectPrototype();

	public slots:
//		virtual QString getName();
//		virtual void setName(QString name);
//		virtual void setPermissions(Permission* permissions);
//		virtual Permission* getPermissions();
//		virtual bool getIsClosed();

		virtual QString ReadAll();
		virtual QString ReadLine();
		virtual bool Write(QString data);
		virtual bool Seek(int pos, Whence whence);
		virtual void Flush();
		virtual void Close();

	private:
		IFileSystemObject* thisObject();
};

#endif // FILESYSTEMOBJECTPROTOTYPE_H
