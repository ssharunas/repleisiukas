#include "localfolderobject.h"
#include <QtScript/QScriptEngine>
#include "../openmodes.h"
#include <QDebug>

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

QString LocalFolderObject::toString()
{
	return "[Folder object]";
}
