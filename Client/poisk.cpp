#include "poisk.h"
#include "ui_poisk.h"

Poisk::Poisk(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Poisk)
{
    ui->setupUi(this);
    http = new obmen;
    ui->lineEdit->setValidator(new QRegExpValidator(QRegExp("[А-я ]{50}")));
    ui->lineCount->setValidator(new QRegExpValidator(QRegExp("[0-9]{4}")));

    connect(http, &obmen::ready, this, &Poisk::DrawButtons);
}

Poisk::~Poisk()
{
    delete ui;
    disconnect(http, &obmen::ready, this, &Poisk::DrawButtons);
    delete http;
}

void Poisk::on_FindButton_clicked()
{
    if(!ui->lineEdit->text().isNull()){
        if (!ui->lineEdit->text().isNull()){
            http->search(ui->lineEdit->text());
        }
        else{
            QMessageBox::critical(this, "Ошибка!", "Не введенно количество продукта!");
        }
    }
    else{
        QMessageBox::critical(this, "Ошибка!", "Не введенно название продукта!");
    }
}

void Poisk::DrawButtons(){
    radioButtons.clear();
    double Count = ui->lineCount->text().toDouble();

    QWidget *controlGroups = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSizeConstraint(QLayout::SetMinAndMaxSize);
    controlGroups->setLayout(layout);
    for (int i = 0; i < http->Info.size(); ++i){
        QRadioButton* btn = new QRadioButton(this);
        QString name = http->Info.at(i).toObject().value("N").toString() + ", " +  QString::number(Count) + "гр." + "\n";
        QString text = + "Каллории: \t" + QString::number(http->Info.at(i).toObject().value("E").toDouble()/100*Count) +
               " | Углеводы: \t" + QString::number(http->Info.at(i).toObject().value("C").toDouble()/100*Count) +
               " | Жиры: \t" + QString::number(http->Info.at(i).toObject().value("F").toDouble()/100*Count)+
               " | Белки: \t" + QString::number(http->Info.at(i).toObject().value("P").toDouble()/100*Count);
        btn->setText(name + text);

        QJsonObject obj;
        obj.insert("N", http->Info.at(i).toObject().value("N").toString() + ", " +  QString::number(Count) + "гр.");
        obj.insert("E", http->Info.at(i).toObject().value("E").toDouble()/100*Count);
        obj.insert("C", http->Info.at(i).toObject().value("C").toDouble()/100*Count);
        obj.insert("F", http->Info.at(i).toObject().value("F").toDouble()/100*Count);
        obj.insert("P", http->Info.at(i).toObject().value("P").toDouble()/100*Count);

        btn->setStatusTip(QJsonDocument(obj).toJson(QJsonDocument::Compact));

        if((name.length()*28) > (text.length()*28)) btn->setMaximumWidth(name.length()*28);
        else btn->setMaximumWidth(text.length()*28);

        radioButtons.append(btn);
        controlGroups->layout()->addWidget(btn);
    }
    ui->scrollArea->setWidget(controlGroups);
    ui->scrollArea->setWidgetResizable(true);

}

void Poisk::on_AddButton_clicked()
{
    for (int i = 0; i < radioButtons.size(); ++i){
        if (radioButtons.at(i)->isChecked()){
            emit AddProduct(radioButtons.at(i)->statusTip());
            ui->lineEdit->clear();
            ui->lineCount->clear();
            ui->scrollArea->widget()->close();
            radioButtons.clear();
            this->close();
            break;
        }
    }
}

void Poisk::on_CloseButton_3_clicked()
{
    emit Close();

    ui->lineEdit->clear();
    ui->lineCount->clear();
    ui->scrollArea->widget()->close();
    radioButtons.clear();
    this->close();

}

