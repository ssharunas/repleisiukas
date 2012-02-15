#include "filesystemobject.h"
#include "filesystemobjectprototype.h"

Q_DECLARE_METATYPE(Permission*)

FileSystemObject::FileSystemObject(QScriptEngine *engine) :
	QObject(engine), QScriptClass(engine)
{
}

FileSystemObject::~FileSystemObject()
{
	name = engine()->toStringHandle("name");
	permissions = engine()->toScriptValue(new Permission(this));
	isClosed = engine()->toScriptValue(false);

	proto = engine()->newQObject(new FileSystemObjectPrototype(this),
								 QScriptEngine::QtOwnership,
								 QScriptEngine::SkipMethodsInEnumeration |
								 QScriptEngine::ExcludeSuperClassMethods |
								 QScriptEngine::ExcludeSuperClassProperties);

	QScriptValue global = engine()->globalObject();
	proto.setPrototype(global.property("Object").property("prototype"));

}
