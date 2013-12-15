#include "qadvancedinputpanel.h"
#include "ui_qadvancedinputpanel.h"
#include <QMenu>
#include <QDebug>
#include <QLineEdit>
#include <QLabel>
#include <QSpinBox>
#include <QSettings>

QAdvancedInputPanel::QAdvancedInputPanel(QWidget *parent) :
	QWidget(parent),
	_formLayout(0),
	_ui(new Ui::QAdvancedInputPanel)
{
	_ui->setupUi(this);

	_ui->page_2->setContextMenuPolicy(Qt::ActionsContextMenu);
	_ui->page_2->addAction(_ui->actionDialogizuoti);
	_ui->textEdit->setContextMenuPolicy(Qt::CustomContextMenu);

	connect(_ui->textEdit,SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(const QPoint &)));
	_formLayout = new QFormLayout(_ui->widget);
	_formLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);
}

QAdvancedInputPanel::~QAdvancedInputPanel()
{
	if(_formLayout != 0){
		delete _formLayout;
		_formLayout = 0;
	}
	delete _ui;
}

void QAdvancedInputPanel::on_actionDialogizuoti_triggered(bool checked)
{
	_ui->stackedWidget->setCurrentIndex(checked);
}

void QAdvancedInputPanel::dialogize()
{
	_ui->actionDialogizuoti->setChecked(true);
	on_actionDialogizuoti_triggered(true);
}

QString QAdvancedInputPanel::toPlainText()
{
	return _ui->textEdit->toPlainText();
}

QString QAdvancedInputPanel::text(){
	QString result;

	if(_ui->stackedWidget->currentIndex() == 0){
		result = _ui->textEdit->toPlainText();
		result = result.replace('\\', "\\\\");
		result = result.replace('\'', "\\'");
		result = result.replace('\n', "\\n");
		result = "wee='" + result +"' ";
	}
	else{
		foreach(QWidget* item, _widgets){
			QString value = getValue(item);
			if(!value.isEmpty()){
				result = result.replace('\\', "\\\\");
				value = value.replace('\'', "\\'");
				value = value.replace('\n', "\\n");

				result += QString("%1 = '%2';\n")
						  .arg(_formLayout->labelForField(item)->property("text").toString())
						  .arg(value);
			}
		}
	}
	return result;
}

void QAdvancedInputPanel::setText(QString text){
	_ui->textEdit->setPlainText(text);
	recreateControls();
}

QString QAdvancedInputPanel::getValue(QWidget* item){
	QLineEdit *line = dynamic_cast<QLineEdit *>(item);
	if(line != 0){
		return line->text();
	}

	QSpinBox *box = dynamic_cast<QSpinBox *>(item);
	if(box != 0){
		return box->cleanText();
	}

	QLabel * label =dynamic_cast<QLabel *>(item);
	if(label != 0){
		return label->text();
	}

	return QString();
}

void QAdvancedInputPanel::clear(){
	_ui->textEdit->clear();
}

void QAdvancedInputPanel::paste(){
	_ui->stackedWidget->setCurrentIndex(0);
	_ui->textEdit->paste();
}

void QAdvancedInputPanel::showContextMenu(const QPoint &pt){
	QMenu *menu = _ui->textEdit->createStandardContextMenu();
	menu->addAction(_ui->actionDialogizuoti);
	menu->exec(_ui->textEdit->mapToGlobal(pt));
	delete menu;
}

void QAdvancedInputPanel::on_stackedWidget_currentChanged(int index)
{
	if(index == 1){
		recreateControls();
	}
}

QAdvancedInputPanel::WidgetTypes QAdvancedInputPanel::getType(QString type){
	if(type.toLower() == "text")
		return Text;
	if(type.toLower() == "number")
		return Number;

	return None;
}

QWidget * QAdvancedInputPanel::createControl(QString strType, QString defVal){
	WidgetTypes type = getType(strType);
	switch(type){
		case Text:
		{
			QLineEdit *line = new QLineEdit(_ui->widget);
			_widgets.append(line);
			line->setText(defVal);
			connect(line, SIGNAL(textChanged(QString)), this, SLOT(onRaiseUpdateRequest()));
			return line;
		}
		case Number:
		{
			QSpinBox *box = new QSpinBox(_ui->widget);
			_widgets.append(box);
			box->setValue(defVal.toInt());
			return box;
		}
		default:
			break;
	}

	QLabel * label = new QLabel(_ui->widget);
	_widgets.append(label);
	label->setText(defVal);
	return label;
}

void QAdvancedInputPanel::clearWidgets(){
	while(_widgets.count()){
		QWidget * tmp = _widgets.takeLast();

		delete _formLayout->labelForField(tmp);
		_formLayout->removeWidget(tmp);
		delete tmp;
		tmp = 0;
	}
}

void QAdvancedInputPanel::recreateControls(){
	QString text = _ui->textEdit->toPlainText();
	if(text != _lastKnownText){
		clearWidgets();

		QStringList splited = text.split(";;\n", QString::SkipEmptyParts);
		for(int i = 0; i < splited.count(); i++){
			QRegExp exp = QRegExp("(\\w+)\\s+([\\w\\d]+)\\s*=\\s*(.+)");
			int index = exp.indexIn(splited[i]);
			Q_UNUSED(index);

			if(exp.captureCount() >= 3){
				QStringList captured = exp.capturedTexts();
				_formLayout->addRow(captured[2], createControl(captured[1], captured[3]));
			}
		}

		_lastKnownText = text;
	}
}

void QAdvancedInputPanel::onRaiseUpdateRequest()
{
	QSettings settings;
	bool autoUpdate = settings.value("/settings/autoUpdate").toBool();

	if(autoUpdate)
		updateRequest();
}

void QAdvancedInputPanel::on_textEdit_textChanged()
{
	onRaiseUpdateRequest();
}
