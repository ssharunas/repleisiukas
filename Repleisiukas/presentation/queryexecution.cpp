#include "queryexecution.h"
#include <QScriptEngine>
#include <QScriptValueIterator>
#include <QDebug>
#include <QSettings>
#include <QApplication>
#include <QDir>
#include <QScriptEngineDebugger>
#include <QScriptValue>

#include "../model/qfilesystemaccess.h"
#include "../model/irepleisiukasscriptobject.h"
#include "../model/filesystem/filesystemobject.h"
#include "../model/filesystem/local/localfolderobject.h"
#include <QList>

Q_DECLARE_METATYPE(IFileSystemObject*)
Q_DECLARE_METATYPE(QList<IFileSystemObject*>)

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
	_fileOperations = 0;
}

FileLoadSave * QueryExecution::fileOperations()
{
	if(_fileOperations == 0)
		_fileOperations = new FileLoadSave(0);

	return _fileOperations;
}

void QueryExecution::attachExtenstions(QScriptEngine &engine)
{
	FileSystemObject::registerMetaType(&engine);
	Permission::registerMetaType(&engine);
	qScriptRegisterSequenceMetaType<QList<IFileSystemObject*> >(&engine);

	QFileSystemAccess::registerGlobalObject(engine);

	engine.globalObject().setProperty("repleisiukas", engine.globalObject());
}

QString QueryExecution::preProxessQuey(QString query)
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

QString QueryExecution::loadExtensions(){
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

QString QueryExecution::execute(QString query, QString userInput)
{
	QString result;

	QString resources = fileOperations()->loadResource(":/scripts/extensions.js");
	QString extensions = loadExtensions();
	query = preProxessQuey(query);

	QString fullQuery = resources + " ;\n" + extensions + " ;\n " + userInput + " ; \"\" ; \n" + query;

	QScriptEngine engine;
	attachExtenstions(engine);

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
		result = "Klaida: " + value.toString();
	}
	else
	{
		result = value.toString();
	}

	return result;
}
