#include "qtabdocument.h"

QTabDocument::QTabDocument(QObject *parent) :
	QObject(parent), _index(-1)
{
}

QTabDocument::~QTabDocument(){
	qDebug("deteted...");
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

QString QTabDocument::input(){
	return _input;
}

QString QTabDocument::query(){
	return _query;
}

QString QTabDocument::output(){
	return _output;
}

//int QTabDocument::index(){
//	return _index;
//}

void QTabDocument::setFileName(QString filename){
	_fileName = filename;
}

void QTabDocument::setName(QString name){
	_name = name;
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

//void QTabDocument::setIndex(int index){
//	_index = index;
//}
