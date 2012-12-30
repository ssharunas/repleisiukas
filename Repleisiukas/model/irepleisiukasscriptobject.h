#ifndef IREPLEISIUKASSCRIPTOBJECT_H
#define IREPLEISIUKASSCRIPTOBJECT_H

#include <QScriptEngine>
#include <QScriptValue>
#include <QScriptable>

class IRepleisiukasScriptObject : public QObject, protected QScriptable
{
    Q_OBJECT
public:
    explicit IRepleisiukasScriptObject(QScriptEngine *engine);
    virtual ~IRepleisiukasScriptObject() {}

    virtual QScriptValue toScriptValue(QScriptEngine *engine);
    virtual void fromScriptValue(const QScriptValue &obj);

protected:
    QScriptValue $this;

private:
    QScriptEngine _engine;
};

#endif // IREPLEISIUKASSCRIPTOBJECT_H
