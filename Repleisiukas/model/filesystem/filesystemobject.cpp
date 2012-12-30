#include "filesystemobject.h"
#include "../scriptvalueconverttemplate.h"

Q_DECLARE_METATYPE(IFileSystemObject*)

FileSystemObject::FileSystemObject(QScriptEngine *engine) :
	IFileSystemObject(engine)
{

}

FileSystemObject::~FileSystemObject()
{
}

void FileSystemObject::registerMetaType(QScriptEngine *engine)
{
	qScriptRegisterMetaType<IFileSystemObject*>(engine,
		&convertToScriptValue<IFileSystemObject>, &convertFromScriptValue<IFileSystemObject>);
}

