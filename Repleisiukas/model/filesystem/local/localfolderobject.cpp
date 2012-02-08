#include "localfolderobject.h"

LocalFolderObject::LocalFolderObject(QScriptEngine *engine) :
	QObject(engine), QScriptEngine(engine)
{
}
