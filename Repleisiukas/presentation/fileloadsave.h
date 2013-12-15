#ifndef FILELOADSAVE_H
#define FILELOADSAVE_H

#include <QObject>
#include <QString>
#include <QTextStream>
#include <QFile>
#include "model/qtabdocument.h"

class FileLoadSave : public QObject
{
		Q_OBJECT
	public:
		explicit FileLoadSave(QWidget *parent = 0);
		~FileLoadSave();

		QString lastQuery();
		void setLastQuery(QString query);

		QString loadFromFile(QString filename);
		QString getLoadFromFileFilename();
		unsigned int menuRecentItemCount();
		void updateLastUsedOrder(QString newName);
		QStringList getLastUsedMenuEntries();
		QString saveToFile(QString query);
		QString saveToFile(QString fileName, QString query);
		QWidget* parent() const;

		QString loadResource(QString path);
		QString getAutoLoadText(QString script);

		void saveCurrentSession(QList<QTabDocument*> &sessionDocuments);
		QList<QTabDocument *> getSavedSession();

	private:
		QTextStream* _strem;
		QFile *_last;

	signals:
		void lastUsedUpdated();
};

#endif // FILELOADSAVE_H
