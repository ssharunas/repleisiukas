#include "qtabdocument.h"

QTabDocument::QTabDocument(QObject *parent) :
	QObject(parent), _isModified(0), _queryScrollPos(0)
{
}

QTabDocument::~QTabDocument(){
	qDebug("Tab was destroyed...");
}

bool QTabDocument::isModified()
{
	return _isModified;
}

QString QTabDocument::uid(){
	if(_uid.isNull())
		_uid = QUuid::createUuid();

	return _uid.toString();
}

QString QTabDocument::fileName(){
	return _fileName;
}

QString QTabDocument::name(){
	return _name;
}

QString QTabDocument::tabName()
{
	QString tabName = name();

	if(!fileName().isEmpty())
	{
		int slash = fileName().lastIndexOf('/');

		if((slash != -1) & (slash + 1 < fileName().length()))
		{
			tabName = fileName().mid(slash + 1);

			if(isModified())
			{
				tabName += "*";
			}
		}
	}

	return tabName;
}

QString QTabDocument::input(){
	return _input;
}

QString QTabDocument::query(){
	return _query;
}

QString QTabDocument::output(){
	return _output;
}

QPoint QTabDocument::queryCursor()
{
	return _queryPos;
}

int QTabDocument::queryScrollPos()
{
	return _queryScrollPos;
}

void QTabDocument::setIsModified(bool value){
	_isModified = value;

	emit changed();
}

void QTabDocument::setFileName(QString filename){
	_fileName = filename;

	emit changed();
}

void QTabDocument::setName(QString name){
	_name = name;

	emit changed();
}

void QTabDocument::setInput(QString input){
	_input = input;
}

void QTabDocument::setQuery(QString query){
	_query = query;
}

void QTabDocument::setOutput(QString output){
	_output = output;
}

void QTabDocument::setQueryCursor(QPoint pos)
{
	_queryPos = pos;
}

void QTabDocument::setQueryScrollPos(int pos)
{
	_queryScrollPos = pos;
}
