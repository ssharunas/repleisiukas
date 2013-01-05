#ifndef LOCALFILEOBJECT_H
#define LOCALFILEOBJECT_H

#include "../filesystemobject.h"
#include "../openmodes.h"

#include <QUrl>
#include <QFile>
#include <QScriptEngine>

class LocalFileObject : public FileSystemObject
{
	Q_OBJECT
	Q_PROPERTY(QString name READ getName WRITE setName)
	Q_PROPERTY(QString path READ getPath)
	Q_PROPERTY(QString content READ getContent WRITE setContent)
	Q_PROPERTY(qint64 pos READ getPos WRITE setPos)
	Q_PROPERTY(bool isEnd READ getIsEnd)
	Q_PROPERTY(Permission* permissions READ getPermissions WRITE setPermissions)

public:
	LocalFileObject(QUrl url, QString mode, QScriptEngine *engine = 0);
	~LocalFileObject();

	virtual QString getName();
	virtual void setName(QString name);

	virtual void setPermissions(Permission* permissions);
	virtual Permission* getPermissions();

	virtual QString getPath();

	QString getContent();
	void setContent(QString content);

	qint64 getPos();
	void setPos(qint64 pos);

	bool getIsEnd();
signals:

public slots:
	qint64 write(QString content);

	QString read(qint64 length);
	QString readLine();

	QString toString();

private:
	bool tryOpenFile(bool writing, bool reading);
	qint64 doWrite(QString content, bool truncate);

private:
	QUrl _url;
	QFile _file;
	QString _mode;
};

#endif // LOCALFILEOBJECT_H
