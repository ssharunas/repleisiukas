#include "localfolderobject.h"
#include "localfileobject.h"

#include <QtScript/QScriptEngine>
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
	if(_mode == MODE_WRITE || _mode == MODE_READ_WRITE)
	{
		QString newName = QDir::cleanPath(_dir.filePath("../" + name));

		if(!QFile::exists(newName)){
			if(!_dir.rename(_dir.absolutePath(), newName)){
				context()->throwError("Rename failed.");
			}else{
				_dir = QDir(newName);
				_url.setPath(newName);
			}
		}else{
			context()->throwError(QString("File or folder with name '%1' already exists.").arg(newName));
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

QString LocalFolderObject::getPath()
{
	return _dir.absolutePath();
}

QList<IFileSystemObject *> LocalFolderObject::getFolders()
{
	QList<IFileSystemObject *> result;

#ifdef Q_WS_WIN
	QFileInfoList folders;

	if(_url.path() == "/")
		folders = _dir.entryInfoList(QDir::Drives | QDir::NoDotAndDotDot);
	else
		folders = _dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System);
#else
	QFileInfoList folders = _dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System);
#endif

	foreach (const QFileInfo folder, folders) {
		result.append(new LocalFolderObject(QUrl(folder.absoluteFilePath()), _mode, _engine));
	}

	return result;
}

QList<IFileSystemObject *> LocalFolderObject::getFiles()
{
	QList<IFileSystemObject *> result;

	QFileInfoList files = _dir.entryInfoList(QDir::Files | QDir::Hidden | QDir::System);
	foreach (const QFileInfo file, files) {
		result.append(new LocalFileObject(QUrl(file.absoluteFilePath()), _mode, _engine));
	}

	return result;
}

QString LocalFolderObject::toString()
{
	QString permissionsStr;
	Permission* permissions = getPermissions();
	if(permissions){
		permissionsStr = permissions->toString() + " ";
		permissions->deleteLater();
	}

	return QString("{D%2%1}").arg(getPath()).arg(permissionsStr);
}
