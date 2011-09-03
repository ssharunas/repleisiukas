#ifndef QADVANCEDINPUTPANEL_H
#define QADVANCEDINPUTPANEL_H

#include <QWidget>
#include <QFormLayout>

namespace Ui {
    class QAdvancedInputPanel;
}

class QAdvancedInputPanel : public QWidget
{
    Q_OBJECT

private:
	enum WidgetTypes{
		None,
		Text,	//text
		Number	//number
	};

public:
    explicit QAdvancedInputPanel(QWidget *parent = 0);
    ~QAdvancedInputPanel();

	QString text();
	void clear();
	void paste();
        void setText(QString text);

signals:
        void updateRequest();

public slots:
	virtual void showContextMenu(const QPoint &pt);
        void Dialogize();

private:
	QFormLayout *formLayout;
    Ui::QAdvancedInputPanel *ui;
	QString lastKnownText;
	QList<QWidget*> widgets;

	void RecreateControls();
	WidgetTypes GetType(QString);
	QWidget * createControl(QString, QString);
	void clearWidgets();
	QString getValue(QWidget*);

private slots:
        void on_textEdit_textChanged();
        void onRaiseUpdateRequest();
        void on_stackedWidget_currentChanged(int );
	void on_actionDialogizuoti_triggered(bool checked);
};

#endif // QADVANCEDINPUTPANEL_H
