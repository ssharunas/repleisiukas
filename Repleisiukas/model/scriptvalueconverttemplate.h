template <typename T>
QScriptValue convertToScriptValue(QScriptEngine *engine, T *const&object)
{
	if(object != 0)
		return object->toScriptValue(engine);

	return engine->nullValue();
}

template <typename T>
void convertFromScriptValue(const QScriptValue &obj, T *&object)
{
	if(object != 0)
		object->fromScriptValue(obj);
}
