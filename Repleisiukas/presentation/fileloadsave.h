#ifndef FILELOADSAVE_H
#define FILELOADSAVE_H

#include <QObject>
#include <QString>
#include <QTextStream>
#include <QFile>

class FileLoadSave : public QObject
{
    Q_OBJECT
public:
	explicit FileLoadSave(QWidget *parent = 0);
	~FileLoadSave();

	QString GetLastQuery();
	void SetLastQuery(QString query);

	QString LoadFromFile(QString filename);
	QString LoadFromFile();
	unsigned int MenuItemCount();
	void UpdateLastUsedOrder(QString newName);
	QStringList GetLastUsedMenuEntries();
	void SaveToFile(QString query);
	void SaveToFile(QString fileName, QString query);
	QWidget* parent() const;

	QString LoadResource(QString path);

private:
	unsigned int DEFAULT_MENU_ITEMS_COUNT;
	QTextStream* strem;
	QFile *last;

signals:
	void UpdateLastUsed();
public slots:

};

#endif // FILELOADSAVE_H
