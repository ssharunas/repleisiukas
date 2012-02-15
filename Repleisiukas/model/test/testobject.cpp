#include "testobject.h"
#include "testprototype.h"
#include <QtScript/QScriptEngine>

Q_DECLARE_METATYPE(QByteArray*)
Q_DECLARE_METATYPE(TestObject*)

TestObject::TestObject(QScriptEngine *engine) :
	QObject(0), QScriptClass(engine), z("da z:)")
{
//	qScriptRegisterMetaType<QByteArray>(engine, toScriptValue, fromScriptValue);

	length = engine->toStringHandle(QLatin1String("length"));

	proto = engine->newQObject(new TestPrototype(this),
							   QScriptEngine::QtOwnership,
							   QScriptEngine::SkipMethodsInEnumeration
							   | QScriptEngine::ExcludeSuperClassMethods
							   | QScriptEngine::ExcludeSuperClassProperties);

	QScriptValue global = engine->globalObject();
	proto.setPrototype(global.property("Object").property("prototype"));

	ctor = engine->newFunction(construct, proto);
	ctor.setData(engine->toScriptValue(this));
}

TestObject::~TestObject()
{
}

QScriptClass::QueryFlags TestObject::queryProperty(const QScriptValue &object,
												   const QScriptString &name,
												   QueryFlags flags, uint *id)
{
	QByteArray *ba = qscriptvalue_cast<QByteArray*>(object.data());
	if (!ba)
		return 0;
	if (name == length) {
		return flags;
	} else {
		bool isArrayIndex;
		qint32 pos = name.toArrayIndex(&isArrayIndex);
		if (!isArrayIndex)
			return 0;
		*id = pos;
		if ((flags & HandlesReadAccess) && (pos >= ba->size()))
			flags &= ~HandlesReadAccess;
		return flags;
	}
}

QScriptValue TestObject::property(const QScriptValue &object,
								  const QScriptString &name, uint id)
{
	QByteArray *ba = qscriptvalue_cast<QByteArray*>(object.data());
	if (!ba)
		return QScriptValue();
	if (name == length) {
		return ba->length();
	} else {
		qint32 pos = id;
		if ((pos < 0) || (pos >= ba->size()))
			return QScriptValue();
		return uint(ba->at(pos)) & 255;
	}
	return QScriptValue();
}

void TestObject::setProperty(QScriptValue &object,
							 const QScriptString &name,
							 uint id, const QScriptValue &value)
{
	QByteArray *ba = qscriptvalue_cast<QByteArray*>(object.data());
	if (!ba)
		return;
	if (name == length)
	{
		resize(*ba, value.toInt32());
	} else {
		qint32 pos = id;
		if (pos < 0)
			return;
		if (ba->size() <= pos)
			resize(*ba, pos + 1);
		(*ba)[pos] = char(value.toInt32());
	}
}

QScriptValue::PropertyFlags TestObject::propertyFlags(
		const QScriptValue &/*object*/, const QScriptString &name, uint /*id*/)
{
	if (name == length) {
		return QScriptValue::Undeletable
				| QScriptValue::SkipInEnumeration;
	}
	return QScriptValue::Undeletable;
}

QString TestObject::name() const
{
	return QLatin1String("ByteArray");
}

QScriptValue TestObject::prototype() const
{
	return proto;
}

QString TestObject::fun2()
{
	return QString("This is another fun from TestObject");
}

QScriptValue TestObject::constructor()
{
	return ctor;
}

QScriptValue TestObject::newInstance()
{
	QScriptValue data = engine()->newVariant(QVariant::fromValue(QByteArray()));
	return engine()->newObject(this, data);
}

QScriptValue TestObject::construct(QScriptContext *ctx, QScriptEngine *)
{
	TestObject *cls = qscriptvalue_cast<TestObject*>(ctx->callee().data());
	if (!cls)
		return QScriptValue();

//	QScriptValue arg = ctx->argument(0);
//	if (arg.instanceOf(ctx->callee()))
//		return cls->newInstance(qscriptvalue_cast<QByteArray>(arg));

	return cls->newInstance();
}

//QScriptValue TestObject::toScriptValue(QScriptEngine *eng, const QByteArray &ba)
//{
//	QScriptValue ctor = eng->globalObject().property("ByteArray");
//	TestObject *cls = qscriptvalue_cast<TestObject*>(ctor.data());

//	if (!cls)
//		return eng->newVariant(QVariant::fromValue(ba));

//	return cls->newInstance();
//}

//void TestObject::fromScriptValue(const QScriptValue &obj, QByteArray &ba)
//{
//	ba = qvariant_cast<QByteArray>(obj.data().toVariant());
//}

void TestObject::resize(QByteArray &ba, int newSize)
{
	ba = QByteArray();
	newSize = 0;
	if(newSize > 0)
		newSize++;
}
