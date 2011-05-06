#include "fileloadsave.h"
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QSettings>
#include <QList>
#include <QFileDialog>

FileLoadSave::FileLoadSave(QWidget *parent) :
    QObject(parent)
{
	DEFAULT_MENU_ITEMS_COUNT = 10;
	strem = 0;

	QSettings settings;
	QString tempFilename;

#ifdef Q_OS_UNIX
	tempFilename = settings.value("/files/tempFile", QVariant("lastQuery")).toString();
#else
	tempFilename = settings.value("/files/tempFile", QVariant("d:\\last.txt")).toString();
#endif

	last = new QFile(tempFilename);
	if (last->open(QIODevice::ReadWrite | QIODevice::Text))
		strem = new QTextStream(last);
	else
		last = 0;
}

FileLoadSave::~FileLoadSave()
{
	if(last != 0){
		last->close();
		if(strem != 0)
			delete strem;
		delete last;
	}
}

QWidget* FileLoadSave::parent() const
{
	return dynamic_cast<QWidget*>(QObject::parent());
}

QString FileLoadSave::GetLastQuery()
{
	QString result;

	if(strem != 0)
	{
		result = strem->readAll();
	}


	return result;
}

void FileLoadSave::SetLastQuery(QString query)
{
	if(last != 0 && strem != 0)
	{
		last->resize(0);

		*strem << query;
		strem->flush();
	}
}

QString FileLoadSave::LoadFromFile()
{
	QSettings settings;
	QString path =  settings.value("/files/lastSaveDir").toString();

	QString fileName = QFileDialog::getOpenFileName(parent(), QString(), path);

	if(!fileName.isEmpty())
	{
#ifdef Q_OS_UNIX
		path = fileName.section('/', 0, -2);
#else
		path = fileName.section('\\', 0, -2);
#endif
		settings.setValue("/files/lastSaveDir", QVariant(path));

		return LoadFromFile(fileName);
	}

	return QString();
}

QString FileLoadSave::LoadFromFile(QString filename)
{
	QString result = QString();

	if(!filename.isEmpty())
	{
		QFile file(filename);
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			QMessageBox::warning(parent(),
				"Could not open file", "could not open file for reading.");
		}else
		{
			QTextStream stream(&file);
			result = stream.readAll();
			UpdateLastUsedOrder(filename);
		}
	}

	return result;
}

unsigned int FileLoadSave::MenuItemCount()
{
	QSettings settings;
	return settings.value("/files/recentCount", QVariant(DEFAULT_MENU_ITEMS_COUNT)).toInt();
}

void FileLoadSave::UpdateLastUsedOrder(QString newName)
{
	QSettings settings;
	int menuItemCount = MenuItemCount();
	QStringList settingsToAdd;
	settingsToAdd.reserve(menuItemCount);

	settingsToAdd.append(newName);

	for(int i = 0; i < menuItemCount - 1; i++)
	{
		QString item = QString("/files/load/%1").arg(i);
		QString shitValue = settings.value(item).toString();

		if(!settingsToAdd.contains(shitValue))
			settingsToAdd.append(shitValue);
	}

	for(int i = 0; i < settingsToAdd.size(); i++)
	{
		QString item = QString("/files/load/%1").arg(i);
		settings.setValue(item, settingsToAdd[i]);
	}

	emit UpdateLastUsed();
}

QStringList FileLoadSave::GetLastUsedMenuEntries()
{
	QStringList result;
	QSettings settings;

	for(unsigned int i = 0; i < MenuItemCount(); i++)
	{
		QString item = settings.value(QString("/files/load/%1").arg(i)).toString();

		if(item.isEmpty())
			break;

		result.append(item);
	}

	return result;
}


void FileLoadSave::SaveToFile(QString query)
{
	QSettings settings;
	QString path =  settings.value("/files/lasLoadDir").toString();
	QString fileName = QFileDialog::getSaveFileName(parent(), QString(), path);

	if(!fileName.isEmpty())
	{
#ifdef Q_OS_UNIX
		path = fileName.section('/', 0, -2);
#else
		path = fileName.section('\\', 0, -2);
#endif
		settings.setValue("/files/lasLoadDir", QVariant(path));

		SaveToFile(fileName, query);
	}
}

void FileLoadSave::SaveToFile(QString fileName, QString query)
{
	if(!fileName.isEmpty())
	{
		QFile file(fileName);

		if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		{
			QMessageBox::warning(parent(),
				"Could not open file", "Could not open file for writing.");
		}

		QTextStream stream(&file);
		stream << query;
		UpdateLastUsedOrder(fileName);
	}
}
