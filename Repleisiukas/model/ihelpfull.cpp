#include "ihelpfull.h"

class IHelpfull::HelpfulProperty{

public:
	HelpfulProperty(const QString name, const QString doc, const QString returns, const QList<IHelpfull::HelphulParam> params);
	HelpfulProperty(const QString name, const QString doc, const QString returns);

	inline bool isFunction() const { return _isFunction; }
	inline QString name() const { return _name; }
	inline QString doc() const { return _doc; }
	inline QString returns() const { return _returns; }
	inline QList<IHelpfull::HelphulParam> params() const { return _params; }
	QString toString(bool shortInfo);

private:
	bool _isFunction;
	QString _name;
	QString _doc;
	QString _returns;
	QList<IHelpfull::HelphulParam> _params;

	QString paramsAsDocString();
	QString paramsAsString();
	QString formatReturns();
	QString formatName();
};

IHelpfull::HelphulParam::HelphulParam(QString name, QString doc)
{
	_name = name;
	_doc = doc;
}

QString IHelpfull::HelphulParam::toString()
{
	return QString("\t\t*%1 - %2").arg(name()).arg(doc());
}


IHelpfull::HelpfulProperty::HelpfulProperty(const QString name, const QString doc, const QString returns, const QList<IHelpfull::HelphulParam> params)
{
	_name = name;
	_doc = doc;
	_returns = returns;
	_params = params;
	_isFunction = true;
}

IHelpfull::HelpfulProperty::HelpfulProperty(const QString name, const QString doc, const QString returns)
{
	_name = name;
	_doc = doc;
	_returns = returns;
	_isFunction = false;
}

QString IHelpfull::HelpfulProperty::paramsAsDocString()
{
	QString paramsStr;

	for(int i = 0; i < params().count(); ++i){
		paramsStr += params()[i].toString() + "\n";
	}

	return paramsStr;
}

QString IHelpfull::HelpfulProperty::paramsAsString()
{
	QString paramsStr;

	for(int i = 0; i < params().count(); ++i){
		if(i)
			paramsStr += ", ";

		paramsStr += params()[i].name();
	}

	return paramsStr;
}

QString IHelpfull::HelpfulProperty::formatReturns()
{
	if(!returns().isEmpty())
		return "Returns: " + returns();

	return QString();
}

QString IHelpfull::HelpfulProperty::formatName()
{
	if(isFunction()){
		return QString::fromUtf8("  • %1(%2)").arg(name(), paramsAsString());
	}

	return QString::fromUtf8("  ◦ ") + name();
}

QString IHelpfull::HelpfulProperty::toString(bool shortInfo){
	QString result;

	result += formatName();
	if(!doc().isEmpty()){
		result += " - " + doc();
	}

	if(!shortInfo){
		QString returns = formatReturns();
		if(!returns.isEmpty())
			result += "\n\t" + returns;

		if(isFunction()){
			QString params = paramsAsDocString();
			if(!params.isEmpty()){
				result += "\n\t" + params;
			}
		}
	}

	result += "\n";

	return result;
}


void IHelpfull::registerFunction(QString name, QString doc, QString returns, QList<IHelpfull::HelphulParam> params)
{
	_properties.insertMulti(name, new IHelpfull::HelpfulProperty(name, doc, returns, params));
}

void IHelpfull::registerProperty(QString name, QString doc, QString returns)
{
	_properties.insertMulti(name, new IHelpfull::HelpfulProperty(name, doc, returns));
}

QString IHelpfull::getHelp(QString funcName)
{
	QString result;
	bool wasFound = false;

	if(!funcName.isEmpty()){
		QHash<QString, HelpfulProperty*>::iterator i = _properties.find(funcName);
		while (i != _properties.end() && i.key() == funcName) {
			result += i.value()->toString(false);
			++i;
			wasFound = true;
		}
	}

	if(!wasFound){
		foreach (HelpfulProperty* value, _properties)
			result += value->toString(true && funcName != "+");
	}

	return result;
}
