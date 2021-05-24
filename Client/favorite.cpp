#include "favorite.h"
#include "ui_favorite.h"

Favorite::Favorite(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Favorite)
{
    ui->setupUi(this);
}

Favorite::~Favorite()
{
    delete ui;
}

void Favorite::ToFavorites(const QJsonObject &data){
    QJsonParseError parseError;
    QJsonObject info;
    const QJsonDocument jsonDoc = QJsonDocument::fromJson(data.value("info").toString().toUtf8(), &parseError);
    if (parseError.error == QJsonParseError::NoError) {
        if (jsonDoc.isObject()){
            info = jsonDoc.object();
            DataToMain = info;
            Name = data.value("name").toString();
        }
    };
    if (info.value("type").toString() == "Breakfast"){
        ui->SaveButton->setText("Добавить в завтрак");
        ui->label->setText("<html><head/><body><p align=\"center\">Избранный завтрак:</p></body></html>");
    }
    else if (info.value("type").toString() == "Snack"){
        ui->SaveButton->setText("Добавить в перекус");
        ui->label->setText("<html><head/><body><p align=\"center\">Избранный перекус:</p></body></html>");
    }
    else if (info.value("type").toString() == "Lunch"){
        ui->SaveButton->setText("Добавить в обед");
        ui->label->setText("<html><head/><body><p align=\"center\">Избранный обед:</p></body></html>");
    }
    else if (info.value("type").toString() == "AfternoonSnack"){
        ui->SaveButton->setText("Добавить в полдник");
        ui->label->setText("<html><head/><body><p align=\"center\">Избранный полдник:</p></body></html>");
    }
    else if (info.value("type").toString() == "Supper"){
        ui->SaveButton->setText("Добавить в ужин");
        ui->label->setText("<html><head/><body><p align=\"center\">Избранный ужин:</p></body></html>");
    }

    QWidget *controlGroups = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSizeConstraint(QLayout::SetMinAndMaxSize);
    controlGroups->setLayout(layout);
    QJsonArray arr = info.value("array").toArray();

    for (int i = 0; i< arr.size(); ++i){
        QCheckBox* btn = new QCheckBox(this);
        QString text = "Каллории: \t" + QString::number(arr.at(i).toObject().value("E").toDouble()) +
                " | Углеводы: \t" + QString::number(arr.at(i).toObject().value("C").toDouble()) + " | Жиры: \t" + QString::number(arr.at(i).toObject().value("F").toDouble())+
                " | Белки: \t" + QString::number(arr.at(i).toObject().value("P").toDouble());
        btn->setText(arr.at(i).toObject().value("N").toString() + "\n" + text);
        btn->setStatusTip(QJsonDocument(arr.at(i).toObject()).toJson(QJsonDocument::Compact));
        btn->setMinimumHeight(35);
        if((arr.at(i).toObject().value("N").toString().length()*28) > (text.length()*28)) btn->setMaximumWidth(arr.at(i).toObject().value("N").toString().length()*28);
        else btn->setMaximumWidth(text.length()*28);

        CheckButtons.append(btn);
    }
    for (int i = 0; i < CheckButtons.size(); ++i){
        controlGroups->layout()->addWidget(CheckButtons.at(i));
    }
    ui->scrollArea_2->setWidget(controlGroups);
    ui->scrollArea_2->setWidgetResizable(true);
}


void Favorite::on_BackButton_clicked()
{
    emit Back();

    ui->scrollArea_2->widget()->close();
    CheckButtons.clear();
    DataToMain = QJsonObject();
    this->close();
}

void Favorite::on_SaveButton_clicked()
{
    emit Save(DataToMain);

    ui->scrollArea_2->widget()->close();
    CheckButtons.clear();
    DataToMain = QJsonObject();
    this->close();
}


void Favorite::on_DelButton_clicked()
{
    emit Del(Name);

    ui->scrollArea_2->widget()->close();
    CheckButtons.clear();
    DataToMain = QJsonObject();
    this->close();
}
