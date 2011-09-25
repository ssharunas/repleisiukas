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
	formLayout(0),
        ui(new Ui::QAdvancedInputPanel)
{
    ui->setupUi(this);

    ui->page_2->setContextMenuPolicy(Qt::ActionsContextMenu);
    ui->page_2->addAction(ui->actionDialogizuoti);
    ui->textEdit->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->textEdit,SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(const QPoint &)));
    formLayout = new QFormLayout(ui->widget);
    formLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);
}

QAdvancedInputPanel::~QAdvancedInputPanel()
{
    if(formLayout != 0){
        delete formLayout;
        formLayout = 0;
    }
    delete ui;
}

void QAdvancedInputPanel::on_actionDialogizuoti_triggered(bool checked)
{
	qDebug() << "trig" << checked;
    ui->stackedWidget->setCurrentIndex(checked);
}

void QAdvancedInputPanel::Dialogize()
{
	//ui->actionDialogizuoti->setChecked(true);
	ui->actionDialogizuoti->trigger();
}

QString QAdvancedInputPanel::toPlainText()
{
	return ui->textEdit->toPlainText();
}

QString QAdvancedInputPanel::text(){
    QString result;

    if(ui->stackedWidget->currentIndex() == 0){
        result = ui->textEdit->toPlainText();
        result = result.replace('\\', "\\\\");
        result = result.replace('\'', "\\'");
        result = result.replace('\n', "\\n");
        result = "wee='" + result +"' ";
    }
    else{
        foreach(QWidget* item, widgets){
            QString value = getValue(item);
            if(!value.isEmpty()){
                result = result.replace('\\', "\\\\");
                value = value.replace('\'', "\\'");
                value = value.replace('\n', "\\n");

                result += QString("%1 = '%2';\n")
                          .arg(formLayout->labelForField(item)->property("text").toString())
                          .arg(value);
            }
        }
    }
    return result;
}

void QAdvancedInputPanel::setText(QString text){
	ui->textEdit->setPlainText(text);
    RecreateControls();
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
    ui->textEdit->clear();
}

void QAdvancedInputPanel::paste(){
    ui->stackedWidget->setCurrentIndex(0);
    ui->textEdit->paste();
}

void QAdvancedInputPanel::showContextMenu(const QPoint &pt){
    QMenu *menu = ui->textEdit->createStandardContextMenu();
    menu->addAction(ui->actionDialogizuoti);
    menu->exec(ui->textEdit->mapToGlobal(pt));
    delete menu;
}

void QAdvancedInputPanel::on_stackedWidget_currentChanged(int index)
{
    if(index == 1){
        RecreateControls();
    }
}

QAdvancedInputPanel::WidgetTypes QAdvancedInputPanel::GetType(QString type){
    if(type.toLower() == "text")
        return Text;
    if(type.toLower() == "number")
        return Number;

    return None;
}

QWidget * QAdvancedInputPanel::createControl(QString strType, QString defVal){
    WidgetTypes type = GetType(strType);
    switch(type){
    case Text:
        {
            QLineEdit *line = new QLineEdit(ui->widget);
            widgets.append(line);
            line->setText(defVal);
            connect(line, SIGNAL(textChanged(QString)), this, SLOT(onRaiseUpdateRequest()));
            return line;
        }
    case Number:
        {
            QSpinBox *box = new QSpinBox(ui->widget);
            widgets.append(box);
            box->setValue(defVal.toInt());
            return box;
        }
    default:
        break;
    }

    QLabel * label = new QLabel(ui->widget);
    widgets.append(label);
    label->setText(defVal);
    return label;
}

void QAdvancedInputPanel::clearWidgets(){
    while(widgets.count()){
        QWidget * tmp = widgets.takeLast();

        delete formLayout->labelForField(tmp);
        formLayout->removeWidget(tmp);
        delete tmp;
        tmp = 0;
    }
}

void QAdvancedInputPanel::RecreateControls(){
    QString text = ui->textEdit->toPlainText();
    if(text != lastKnownText){
        clearWidgets();

        QStringList splited = text.split(";;\n", QString::SkipEmptyParts);
        for(int i = 0; i < splited.count(); i++){
            QRegExp exp = QRegExp("(\\w+)\\s+([\\w\\d]+)\\s*=\\s*(.+)");
            int index = exp.indexIn(splited[i]);
			Q_UNUSED(index);

            if(exp.captureCount() >= 3){
                QStringList captured = exp.capturedTexts();
                formLayout->addRow(captured[2], createControl(captured[1], captured[3]));
            }

//            qDebug() << index << exp.captureCount() << exp.capturedTexts();
        }

        lastKnownText = text;
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
