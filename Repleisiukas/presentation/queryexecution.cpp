#include "queryexecution.h"
#include <QScriptEngine>
#include <QScriptValueIterator>
#include <QDebug>
#include <QSettings>
#include <QApplication>
#include <QDir>
#include <QScriptEngineDebugger>

QueryExecution::QueryExecution(QObject *parent) :
	QObject(parent), _fileOperations(0)
{

}

QueryExecution::QueryExecution(FileLoadSave* fileOperations, QObject *parent) :
	QObject(parent)
{
	_fileOperations = fileOperations;
}

QueryExecution::~QueryExecution(){
	if(_fileOperations != 0)
		delete _fileOperations;

	_fileOperations = 0;
}

FileLoadSave * QueryExecution::FileOperations()
{
	if(_fileOperations == 0)
		_fileOperations = new FileLoadSave(0);

	return _fileOperations;
}

QString QueryExecution::PreProxessQuey(QString query)
{
	QString result;
	bool changed = false;
	int lastIndex = 0;
	do
	{
		changed = false;
		int from = query.indexOf("\"\"\"", lastIndex);
		int to = query.indexOf("\"\"\"", from + 1);
		if(from >= 0 && to - from >= 3)
		{
			result += query.mid(lastIndex, from - lastIndex);

			result += "('";
			result += query.mid(from + 3, to - 3 - from).replace('\'', "\\'").replace('\r', "").replace('\n', "\\n' + \n'");
			result += "')";
			lastIndex = to + 3;

			changed = true;
		}
	}while(changed);

	result += query.mid(lastIndex);

	return result;
}

QString QueryExecution::LoadExtensions(){
	QString result;

	QSettings settings;
	QString extensionsPath =
			settings.value("/settings/extensionsPath", QApplication::applicationDirPath() + "/ext").toString();

	QDir dir(extensionsPath);
	if(dir.exists())
	{
		QFileInfoList list = dir.entryInfoList(QDir::Files);
		for (int i = 0; i < list.size(); ++i)
		{
			QFileInfo fileInfo = list.at(i);

			QFile file(fileInfo.filePath());

			if (file.open(QIODevice::ReadOnly | QIODevice::Text))
			{
				QTextStream stream(&file);
				result += stream.readAll();

				result += "\n ; ";
				file.close();
			}
		}
	}

	return result;
}

QString QueryExecution::Execute(QString query, QString userInput)
{
	QString result;

	QString resources = FileOperations()->LoadResource(":/scripts/extensions.js");
	QString extensions = LoadExtensions();
	query = PreProxessQuey(query);

	QString fullQuery = resources + QString(" ; %3 ;\n %1 ; \n%2")
			.arg(userInput)
			.arg(query)
			.arg(extensions);

	qDebug() << fullQuery;

	QScriptEngine engine;

	QSettings settings;
	bool debugger = settings.value("/settings/debugger").toBool();
	if(debugger)
	{
		QScriptEngineDebugger *debuger = new QScriptEngineDebugger(&engine);
		debuger->attachTo(&engine);
	}

	QScriptValue value = engine.evaluate(fullQuery);

	if(value.isError())
	{
		QScriptValueIterator it(value);
		while (it.hasNext()) {
			it.next();
			qDebug() << it.name() << ": " << it.value().toString();
		}

		result = "Klaida: " + value.toString();
	}
	else
	{
		result = value.toString();
	}

	return result;
}
