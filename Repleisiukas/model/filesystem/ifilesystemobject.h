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
	public:
		IFileSystemObject(QScriptEngine* engine) : IRepleisiukasScriptObject(engine) {}
		virtual ~IFileSystemObject() {}

		virtual QString getName() = 0;
		virtual void setName(QString name) = 0;
		virtual void setPermissions(Permission* permissions) = 0;
		virtual Permission* getPermissions() = 0;
		virtual QString getPath() = 0;

//		virtual bool getIsClosed() = 0;

//		virtual QString ReadAll() = 0;
//		virtual QString ReadLine() = 0;
//		virtual bool Write(QString data) = 0;
//		virtual bool Seek(int pos, Whence whence) = 0;
//		virtual void Flush() = 0;
//		virtual void Close() = 0;

//		virtual void registerMetaType(QScriptEngine *engine){
//			Q_DECLARE_METATYPE(IFileSystemObject)
//			typedef IFileSystemObject* pIFileSystemObject;

//			qScriptRegisterMetaType(engine, &convertToScriptValue<pIFileSystemObject>, &convertFromScriptValue<pIFileSystemObject>);
//		}
};

#endif // IFILESYSTEMOBJECT_H
