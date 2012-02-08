#ifndef QFILESYSTEMACCESS_H
#define QFILESYSTEMACCESS_H

#include "filesystem/ifilesystemobject.h"
#include <QObject>
#include <QtScript/QScriptClass>
#include <QtScript/QScriptString>

class QFileSystemAccess : public QObject, public QScriptClass
{
		Q_OBJECT
	public:
		explicit QFileSystemAccess(QScriptEngine *engine = 0);

	signals:

	public slots:
		IFileSystemObject* open(QString path, QString mode);
		bool exists(QString path);

	private:
		QString getScheme(QString path);
	private:
		QScriptValue proto;
};

#endif // QFILESYSTEMACCESS_H
