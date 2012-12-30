#include "localfolderobject.h"
#include <QtScript/QScriptEngine>
#include "../openmodes.h"
#include <QDebug>
#include <QFileInfo>

Q_DECLARE_METATYPE(LocalFolderObject*)

LocalFolderObject::LocalFolderObject(QUrl url, QString mode, QScriptEngine *engine) :
	FileSystemObject(engine)
{
	if(is_valid_open_mode(mode)){
		_mode = mode;
	}else{
		_mode = MODE_READ;
	}

	_url = url;
	_dir = QDir(_url.path());
}

LocalFolderObject::~LocalFolderObject() { }

QString LocalFolderObject::getName()
{
	return _dir.dirName();
}

void LocalFolderObject::setName(QString name)
{
	if(_mode == MODE_WRITE)
	{
		QString newName = QDir::cleanPath(_dir.filePath("../" + name));
		qDebug() << _dir.absolutePath() << newName;

		if(!_dir.rename(_dir.absolutePath(), newName)){
			context()->throwError("Rename failed.");
		}else{
			_dir = QDir(newName);
			_url.setPath(newName);
		}
	}else{
		context()->throwError("Could not rename. Directry must be opened in write mode ('w')");
	}
}

void LocalFolderObject::setPermissions(Permission *permissions)
{
	Q_UNUSED(permissions)
}

Permission *LocalFolderObject::getPermissions()
{
	Permission* permission = 0;
	QFileInfo fileInfo(_url.path());
	if(fileInfo.exists()){
		permission = new Permission(_engine);

		permission->uR = fileInfo.permission(QFile::ReadOwner);
		permission->gR = fileInfo.permission(QFile::ReadGroup);
		permission->oR = fileInfo.permission(QFile::ReadOther);

		permission->uW = fileInfo.permission(QFile::WriteOwner);
		permission->gW = fileInfo.permission(QFile::WriteGroup);
		permission->oW = fileInfo.permission(QFile::WriteOther);

		permission->uX = fileInfo.permission(QFile::ExeOwner);
		permission->gX = fileInfo.permission(QFile::ExeGroup);
		permission->oX = fileInfo.permission(QFile::ExeOther);

		permission->user = fileInfo.owner();
		permission->uid = fileInfo.ownerId();
		permission->group = fileInfo.group();
		permission->gid = fileInfo.groupId();
	}


	return permission;
}

QString LocalFolderObject::toString()
{
	return "[Folder object]";
}
