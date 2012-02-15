#ifndef TESTOBJECT_H
#define TESTOBJECT_H

#include <QtCore/QObject>
#include <QtScript/QScriptClass>
#include <QtScript/QScriptString>
#include <QtScript/QScriptContext>

class TestObject :  public QObject, public QScriptClass
{
		Q_OBJECT
		Q_PROPERTY(QString ZZZ READ ZZZ WRITE setZZZ)
	public:
		explicit TestObject(QScriptEngine *engine);
		~TestObject();

		QScriptValue constructor();
		QScriptValue newInstance();

		QueryFlags queryProperty(const QScriptValue &object,
								 const QScriptString &name,
								 QueryFlags flags, uint *id);

		QScriptValue property(const QScriptValue &object,
							  const QScriptString &name, uint id);

		void setProperty(QScriptValue &object, const QScriptString &name,
						 uint id, const QScriptValue &value);

		QScriptValue::PropertyFlags propertyFlags(
				const QScriptValue &object, const QScriptString &name, uint id);


		QString name() const;



		QScriptValue prototype() const;

		void setZZZ(QString zz){z = zz;}
		QString ZZZ(){return z;}

		QString fun2();

	private:
		static QScriptValue construct(QScriptContext *ctx, QScriptEngine *eng);

//		static QScriptValue toScriptValue(QScriptEngine *eng, const QByteArray &ba);
//		static void fromScriptValue(const QScriptValue &obj, QByteArray &ba);

		void resize(QByteArray &ba, int newSize);

		QScriptString length;
		QScriptValue proto;
		QScriptValue ctor;

		QString z;
		
};

#endif // TESTOBJECT_H
