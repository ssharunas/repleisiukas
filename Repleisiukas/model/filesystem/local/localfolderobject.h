#ifndef LOCALFOLDEROBJECT_H
#define LOCALFOLDEROBJECT_H

#include <QScriptEngine>

class LocalFolderObject : public QObject, public QScriptClass
{
//		Q_OBJECT
	public:
		LocalFolderObject(QScriptEngine *engine);
		~LocalFolderObject();

		
	signals:
		
	public slots:
		
};

#endif // LOCALFOLDEROBJECT_H
