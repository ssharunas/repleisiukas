#include "filesystemobjectprototype.h"

#include <QtScript/QScriptClass>

FileSystemObjectPrototype::FileSystemObjectPrototype(QObject* parent) :
	QObject(parent)
{

}

FileSystemObjectPrototype::~FileSystemObjectPrototype()
{

}
/*
QString FileSystemObjectPrototype::getName()
{
	QString result;

	IFileSystemObject* item = thisObject();

	if(item)
		result = item->getName();

	item = 0;

	return result;
}

void FileSystemObjectPrototype::setName(QString name)
{
	IFileSystemObject* item = thisObject();

	if(item)
		item->setName(name);

	item = 0;
}

void FileSystemObjectPrototype::setPermissions(Permission* permissions)
{
	IFileSystemObject* item = thisObject();

	if(item)
		item->setPermissions(permissions);

	item = 0;
}

Permission* FileSystemObjectPrototype::getPermissions()
{
	Permission* result = 0;

	IFileSystemObject* item = thisObject();

	if(item)
		result = item->getPermissions();

	item = 0;

	return result;
}

bool FileSystemObjectPrototype::getIsClosed()
{
	bool result = false;

	IFileSystemObject* item = thisObject();

	if(item)
		result = item->getIsClosed();

	item = 0;

	return result;
}
*/

QString FileSystemObjectPrototype::ReadAll()
{
	QString result;

	IFileSystemObject* item = thisObject();

	if(item)
		result = item->ReadAll();

	item = 0;

	return result;
}

QString FileSystemObjectPrototype::ReadLine()
{
	QString result;

	IFileSystemObject* item = thisObject();

	if(item)
		result = item->ReadLine();

	item = 0;

	return result;
}

bool FileSystemObjectPrototype::Write(QString data)
{
	bool result = false;

	IFileSystemObject* item = thisObject();

	if(item)
		result = item->Write(data);

	item = 0;

	return result;
}

bool FileSystemObjectPrototype::Seek(int pos, Whence whence)
{
	bool result = false;

	IFileSystemObject* item = thisObject();

	if(item)
		result = item->Seek(pos, whence);

	item = 0;

	return result;
}

void FileSystemObjectPrototype::Flush()
{
	IFileSystemObject* item = thisObject();

	if(item)
		item->Flush();

	item = 0;
}

void FileSystemObjectPrototype::Close()
{
	IFileSystemObject* item = thisObject();

	if(item)
		item->Close();

	item = 0;
}

IFileSystemObject *FileSystemObjectPrototype::thisObject()
{
	IFileSystemObject *result = 0;

	QScriptClass * object = QScriptable::thisObject().scriptClass();
	if(object)
	{
		result = dynamic_cast<IFileSystemObject*>(object);

		object = 0;
	}

	return result;
}
