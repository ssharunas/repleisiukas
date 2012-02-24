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

		QString GetLastQuery();
		void SetLastQuery(QString query);

		QString LoadFromFile(QString filename);
		QString GetLoadFromFileFilename();
		unsigned int MenuItemCount();
		void UpdateLastUsedOrder(QString newName);
		QStringList GetLastUsedMenuEntries();
		QString SaveToFile(QString query);
		QString SaveToFile(QString fileName, QString query);
		QWidget* parent() const;

		QString LoadResource(QString path);
		QString GetAutoLoadText(QString script);

		void saveCurrentSession(QList<QTabDocument*> &sessionDocuments);
		QList<QTabDocument *> getSavedSession();

	private:
		unsigned int DEFAULT_MENU_ITEMS_COUNT;
		QTextStream* strem;
		QFile *last;

	signals:
		void UpdateLastUsed();
	public slots:

};

#endif // FILELOADSAVE_H
