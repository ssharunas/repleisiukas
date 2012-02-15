#ifndef TESTPROTOTYPE_H
#define TESTPROTOTYPE_H

#include <QtCore/QByteArray>
 #include <QtCore/QObject>
 #include <QtScript/QScriptable>
 #include <QtScript/QScriptValue>

class TestPrototype : public QObject, public QScriptable
{
		Q_OBJECT
	public:
		explicit TestPrototype(QObject *parent = 0);
		 ~TestPrototype();

	public slots:
		QString fun();
		
};

#endif // TESTPROTOTYPE_H
