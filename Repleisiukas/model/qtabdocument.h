#ifndef QTABDOCUMENT_H
#define QTABDOCUMENT_H

#include <QObject>
#include <QUuid>

class QTabDocument : public QObject
{
		Q_OBJECT
	public:
		explicit QTabDocument(QObject *parent = 0);

		QString uid();
		QString fileName();
		QString name();

		QString input();
		QString query();
		QString output();
//		int index();

		void setFileName(QString);
		void setName(QString);

		void setInput(QString);
		void setQuery(QString);
		void setOutput(QString);
//		void setIndex(int);
	private:
		int _index;
		QUuid _uid;
		QString _fileName;
		QString _name;

		QString _input;
		QString _query;
		QString _output;

};

#endif // QTABDOCUMENT_H
