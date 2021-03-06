#ifndef LOCALFOLDEROBJECT_H
#define LOCALFOLDEROBJECT_H

#include "../filesystemobject.h"
#include "../openmodes.h"

#include <QUrl>
#include <QDir>
#include <QScriptEngine>

class LocalFolderObject : public FileSystemObject
{
	Q_OBJECT
	Q_PROPERTY(QString name READ getName WRITE setName)
	Q_PROPERTY(QString path READ getPath)
	Q_PROPERTY(QList<IFileSystemObject*> folders READ getFolders)
	Q_PROPERTY(QList<IFileSystemObject*> files READ getFiles)
	Q_PROPERTY(Permission* permissions READ getPermissions WRITE setPermissions)

	public:
		LocalFolderObject(QUrl url, QString mode, QScriptEngine *engine = 0);
		~LocalFolderObject();

		virtual QString getName();
		virtual void setName(QString name);

		virtual void setPermissions(Permission* permissions);
		virtual Permission* getPermissions();

		virtual QString getPath();
		QList<IFileSystemObject *> getFolders();
		QList<IFileSystemObject *> getFiles();

	signals:

	public slots:
		QString toString();

	private:
		QUrl _url;
		QDir _dir;
		QString _mode;
};

#endif // LOCALFOLDEROBJECT_H
