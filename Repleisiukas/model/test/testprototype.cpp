#include "testprototype.h"
#include <QtScript/QScriptEngine>
#include "testobject.h"
#include <QDebug>

Q_DECLARE_METATYPE(TestObject*)

TestPrototype::TestPrototype(QObject *parent) :
    QObject(parent)
{
}

TestPrototype::~TestPrototype()
{
}

QString TestPrototype::fun()
{
//	qDebug() << thisObject().data();
	QString r = QString("This is fun TestPrototype.") + thisObject().data().toString();

	TestObject *cls = dynamic_cast<TestObject *>(thisObject().scriptClass());
	if(cls)
		r += cls->name();
	else
		r += "NULL";

	return r;
}

