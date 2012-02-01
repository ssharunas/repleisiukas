#ifndef QFILESYSTEMACCESS_H
#define QFILESYSTEMACCESS_H

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

	private:
		 QScriptValue proto;
};

#endif // QFILESYSTEMACCESS_H
