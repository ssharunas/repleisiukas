#include "filesystemobject.h"

Q_DECLARE_METATYPE(Permission*)

FileSystemObject::FileSystemObject(QScriptEngine *engine) :
    IFileSystemObject(engine)
{

}

FileSystemObject::~FileSystemObject()
{
}
