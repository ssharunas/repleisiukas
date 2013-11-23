#ifndef QTABDOCUMENT_H
#define QTABDOCUMENT_H

#include <QObject>
#include <QUuid>
#include <QPoint>

class QTabDocument : public QObject
{
		Q_OBJECT
	public:
		explicit QTabDocument(QObject *parent = 0);
		~QTabDocument();

		bool isModified();
		QString uid();
		QString fileName();
		QString name();
		QString tabName();

		QString input();
		QString query();
		QString output();
		QPoint queryCursor();
		int queryScrollPos();

		void setIsModified(bool);
		void setFileName(QString);
		void setName(QString);

		void setInput(QString);
		void setQuery(QString);
		void setOutput(QString);
		void setQueryCursor(QPoint);
		void setQueryScrollPos(int);

	signals:
		void changed();

	private:
		bool _isModified;
		QUuid _uid;
		QString _fileName;
		QString _name;

		QString _input;
		QString _query;
		QString _output;
		QPoint _queryPos;
		int _queryScrollPos;
};

#endif // QTABDOCUMENT_H
