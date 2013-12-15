#ifndef IFILESYSTEMOBJECT_H
#define IFILESYSTEMOBJECT_H

#include "whence.h"
#include "permission.h"
#include "../irepleisiukasscriptobject.h"

#include <QString>
#include <QScriptValue>
#include <QScriptEngine>

class IFileSystemObject : public IRepleisiukasScriptObject
{
#if QT_VERSION >= 0x050000
	Q_OBJECT
#endif

	public:
		IFileSystemObject(QScriptEngine* engine) : IRepleisiukasScriptObject(engine) {}
		virtual ~IFileSystemObject() {}

		virtual QString getName() = 0;
		virtual void setName(QString name) = 0;
		virtual void setPermissions(Permission* permissions) = 0;
		virtual Permission* getPermissions() = 0;
		virtual QString getPath() = 0;
};

#endif // IFILESYSTEMOBJECT_H
