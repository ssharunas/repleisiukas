#ifndef LOCALFOLDEROBJECT_H
#define LOCALFOLDEROBJECT_H

#include "../filesystemobject.h"

#include <QUrl>
#include <QDir>
#include <QScriptEngine>

class LocalFolderObject : public FileSystemObject
{
	Q_OBJECT
	Q_PROPERTY(QString name READ getName WRITE setName)

	public:
		LocalFolderObject(QUrl url, QString mode, QScriptEngine *engine = 0);
		~LocalFolderObject();

		virtual QString getName();
		virtual void setName(QString name);

	signals:

	public slots:
		QString toString();

	private:
		QUrl _url;
		QDir _dir;
		QString _mode;
};

#endif // LOCALFOLDEROBJECT_H
