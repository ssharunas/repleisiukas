#include "localfileobject.h"
#include <QtScript/QScriptEngine>
#include <QDebug>
#include <QFileInfo>
#include <QDir>

LocalFileObject::LocalFileObject(QUrl url, QString mode, QScriptEngine *engine) :
	FileSystemObject(engine), _file(url.path())
{
	if(is_valid_open_mode(mode)){
		_mode = mode;
	}else{
		_mode = MODE_READ;
	}

	_url = url;
}

LocalFileObject::~LocalFileObject() { }


QString LocalFileObject::getName()
{
	QFileInfo fileInfo(_file.fileName());
	return fileInfo.fileName();
}

void LocalFileObject::setName(QString name)
{
	if(_mode == MODE_WRITE || _mode == MODE_READ_WRITE)
	{
		QFileInfo fileInfo(_file.fileName());
		QString newName = QDir::cleanPath(fileInfo.absolutePath() + "/" + name);

		if(!QFile::exists(newName)){
			if(!_file.rename(newName)){
				context()->throwError("Rename failed.");
			}else{
				_url.setPath(newName);
			}
		}else{
			context()->throwError(QString("File or folder with name '%1' already exists.").arg(newName));
		}
	}else{
		context()->throwError("Could not rename. File must be opened in write mode ('w')");
	}
}

void LocalFileObject::setPermissions(Permission *permissions)
{
	Q_UNUSED(permissions)
}

Permission *LocalFileObject::getPermissions()
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

QString LocalFileObject::getPath()
{
	QFileInfo fileInfo(_file.fileName());
	return fileInfo.absolutePath();
}

QString LocalFileObject::getContent()
{
	if(_mode == MODE_READ || _mode == MODE_READ_WRITE || _mode == MODE_APPEND){
		if(!tryOpenFile(false, true)){
			return QString();
		}

		return _file.readAll();

	}else{
		context()->throwError("Could get content. File must be opened in read ('r') or append ('a') mode.");
	}

	return QString();
}

void LocalFileObject::setContent(QString content)
{
	doWrite(content, true);
}

qint64 LocalFileObject::getPos()
{
	if(tryOpenFile(true, true))
		return _file.pos();

	return -1;
}

void LocalFileObject::setPos(qint64 pos)
{
	if(tryOpenFile(true, true))
		_file.seek(pos);
}

bool LocalFileObject::getIsEnd()
{
	if(tryOpenFile(true, true))
		return _file.atEnd();

	return true;
}

QString LocalFileObject::toString()
{
	QString permissionsStr;
	Permission* permissions = getPermissions();
	if(permissions){
		permissionsStr = permissions->toString() + " ";
		permissions->deleteLater();
	}

	return QString("{F%2%1}").arg(_file.fileName()).arg(permissionsStr);
}

bool LocalFileObject::tryOpenFile(bool writing, bool reading)
{
	bool result = true;

	if(!_file.isOpen()){
		if(!writing && !reading) {
			result = false;
		}
		else {
			QIODevice::OpenMode mode;

			if(writing && _mode == MODE_WRITE) {
				mode = QFile::WriteOnly;
			}
			else if(writing && _mode == MODE_APPEND) {
				mode = QFile::Append;
			}
			else if(reading && _mode == MODE_READ) {
				mode = QFile::Append;
			}
			else if((reading || writing) && _mode == MODE_READ_WRITE) {
				mode = QFile::ReadWrite;
			}
			else {
				result = false;
			}

			if(result)
				result = _file.open(mode);
		}
	}

	if(!result)
		context()->throwError(QString("Failed to open file in mode '%1'.").arg(_mode));

	return true;
}

qint64 LocalFileObject::doWrite(QString content, bool truncate)
{
	qint64 result = -1;

	if(_mode == MODE_WRITE || _mode == MODE_APPEND || _mode == MODE_READ_WRITE){
		if(!tryOpenFile(true, false)){
			return -1;
		}

		if(truncate)
			_file.seek(0);

		result = _file.write(content.toUtf8());

		if(truncate)
			_file.resize(_file.pos());

	}else{
		context()->throwError("Could set content. File must be opened in write ('w') or append ('a') mode.");
	}

	return result;
}


qint64 LocalFileObject::write(QString content)
{
	return doWrite(content, false);
}

QString LocalFileObject::read(qint64 length)
{
	QString result;

	if(tryOpenFile(false, true)){
		char buf[length];

		qint64 dataLength = _file.read(buf, sizeof(buf));

		if(dataLength > 0)
			result = QByteArray(buf, dataLength);
	}

	return result;
}

QString LocalFileObject::readLine()
{
	QString result;

	if(tryOpenFile(false, true)){
		const int BUFFER_SIZE = 1024;
		char buf[BUFFER_SIZE + 1];

		qint64 lineLength = BUFFER_SIZE;
		int infinityPrevent = 1024;

		while(lineLength == BUFFER_SIZE && infinityPrevent > 0){
			infinityPrevent--;

			lineLength = _file.readLine(buf, sizeof(buf));

			if(lineLength > 0)
				result += QByteArray(buf, lineLength);
		}
	}

	return result;
}
