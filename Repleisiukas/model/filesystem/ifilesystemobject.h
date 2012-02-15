#ifndef IFILESYSTEMOBJECT_H
#define IFILESYSTEMOBJECT_H

#include "whence.h"
#include "permission.h"
#include <QString>

class IFileSystemObject
{
	public:
		virtual ~IFileSystemObject() {}

		virtual QString getName() = 0;
		virtual void setName(QString name) = 0;
		virtual void setPermissions(Permission* permissions) = 0;
		virtual Permission* getPermissions() = 0;
		virtual bool getIsClosed() = 0;

		virtual QString ReadAll() = 0;
		virtual QString ReadLine() = 0;
		virtual bool Write(QString data) = 0;
		virtual bool Seek(int pos, Whence whence) = 0;
		virtual void Flush() = 0;
		virtual void Close() = 0;
};

#endif // IFILESYSTEMOBJECT_H
