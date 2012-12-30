#include "irepleisiukasscriptobject.h"

IRepleisiukasScriptObject::IRepleisiukasScriptObject(QScriptEngine *engine) :
    QObject(engine), _engine(engine)
{
}

QScriptValue IRepleisiukasScriptObject::toScriptValue(QScriptEngine *engine)
{
    if(!$this.isValid())
        $this = engine->newQObject(this, QScriptEngine::ScriptOwnership, QScriptEngine::ExcludeSuperClassContents);

    return $this;
}

void IRepleisiukasScriptObject::fromScriptValue(const QScriptValue &obj)
{
    Q_UNUSED(obj);
}
