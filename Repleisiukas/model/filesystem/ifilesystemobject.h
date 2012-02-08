#ifndef IFILESYSTEMOBJECT_H
#define IFILESYSTEMOBJECT_H

#include "whence.h"
#include <QString>

class IFileSystemObject
{
		virtual ~IFileSystemObject() {}

		virtual QString getName() = 0;
		virtual QString setName() = 0;
		virtual QString setPermissions() = 0;
		virtual QString getPermissions() = 0;
		virtual bool setIsClosed() = 0;
		virtual bool getIsClosed() = 0;

		virtual QString ReadAll() = 0;
		virtual QString ReadLine() = 0;
		virtual void Write(QString data) = 0;
		virtual void Seek(int pos, Whence whence) = 0;
		virtual void Flush() = 0;
		virtual void Close() = 0;
};

#endif // IFILESYSTEMOBJECT_H
