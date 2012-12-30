#ifndef QFILESYSTEMACCESS_H
#define QFILESYSTEMACCESS_H

#include "filesystem/ifilesystemobject.h"
#include <QObject>
#include <QStringList>
#include <QtScript/QScriptClass>
#include <QtScript/QScriptString>

class QFileSystemAccess : public QObject
{
		Q_OBJECT
	public:
        explicit QFileSystemAccess(QScriptEngine *engine);

	public slots:
		IFileSystemObject* open(QString path, QString mode);
		bool exists(QString path);
        QString cwd();
		QString toString();
        QStringList schemes();

	private:
        QScriptEngine* _engine;
};

#endif // QFILESYSTEMACCESS_H
