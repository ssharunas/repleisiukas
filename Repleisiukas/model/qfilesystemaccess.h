#ifndef QFILESYSTEMACCESS_H
#define QFILESYSTEMACCESS_H

#include "filesystem/ifilesystemobject.h"
#include "ihelpfull.h"
#include <QObject>
#include <QStringList>
#include <QtScript/QScriptClass>
#include <QtScript/QScriptString>

class QFileSystemAccess : public QObject, public IHelpfull
{
		Q_OBJECT
		Q_PROPERTY(QString cwd READ cwd)
	public:
		explicit QFileSystemAccess(QScriptEngine *engine);

	public slots:
		IFileSystemObject* open(QString path);
		IFileSystemObject* open(QString path, QString mode);
		bool exists(QString path);
		QStringList schemes();

		virtual QString help();
		virtual QString help(QString param);

		QString toString();
	public:
		QString cwd();

	private:
		void initHelp();
		QScriptEngine* _engine;
};

#endif // QFILESYSTEMACCESS_H
