#ifndef IHELPFULL_H
#define IHELPFULL_H

#include <QString>
#include <QList>
#include <QHash>

class IHelpfull
{
public:
	virtual ~IHelpfull() {}

	virtual QString help() = 0;
	virtual QString help(QString) = 0;

protected:

	class HelphulParam{
		public:
			HelphulParam(QString name, QString doc);

			inline QString name(){ return _name;}
			inline QString doc(){ return _doc;}
			QString toString();

		private:
			QString _name;
			QString _doc;
	};

	void registerFunction(QString name, QString doc, QString returns, QList<HelphulParam> params);
	void registerProperty(QString name, QString doc, QString returns);
	QString getHelp(QString funcName);

private:
	class HelpfulProperty;
	QHash<QString, HelpfulProperty*> _properties;
};

#endif // IHELPFULL_H
