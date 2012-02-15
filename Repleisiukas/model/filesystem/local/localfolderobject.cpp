#include "localfolderobject.h"
 #include <QtScript/QScriptEngine>

Q_DECLARE_METATYPE(LocalFolderObject*)

LocalFolderObject::LocalFolderObject(QScriptEngine *engine) :
	FileSystemObject(engine)
{
//	ctor = engine->newFunction(constreuct, proto);
	ctor.setData(engine->toScriptValue(this));
}

LocalFolderObject::~LocalFolderObject()
{
}
