#ifndef QUERYEXECUTION_H
#define QUERYEXECUTION_H

#include <QObject>
#include <QScriptEngine>
#include "fileloadsave.h"

class QueryExecution : public QObject
{
		Q_OBJECT
	public:
		explicit QueryExecution(QObject *parent = 0);
		explicit QueryExecution(FileLoadSave* fileOperations, QObject *parent = 0);
		~QueryExecution();

	private:
		QString preProxessQuey(QString query);
		QString loadExtensions();
		FileLoadSave * fileOperations();
		void attachExtenstions(QScriptEngine &engine);

	private:
		FileLoadSave * _fileOperations;

	public slots:
		QString execute(QString query, QString userInput);

};

#endif // QUERYEXECUTION_H
