#include "adder.h"
#include "ui_adder.h"

adder::adder(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::adder)
{
    ui->setupUi(this);
    finder = new Poisk;
    connect(finder, &Poisk::Close, this, &adder::show);
    connect(finder, &Poisk::AddProduct, this, &adder::show);
    connect(finder, SIGNAL(AddProduct(QString)), this, SLOT(GetProduct(QString)));
}

adder::~adder()
{
    delete ui;
}

void adder::GetProduct(const QString &product){
    QWidget *controlGroups = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSizeConstraint(QLayout::SetMinAndMaxSize);
    controlGroups->setLayout(layout);

    const QJsonDocument jsonDoc = QJsonDocument::fromJson(product.toUtf8());
    QJsonObject jsonObj = jsonDoc.object();

    QCheckBox* btn = new QCheckBox(this);
    QString text = "Каллории: \t" + QString::number(jsonObj.value("E").toDouble()) +
            " | Углеводы: \t" + QString::number(jsonObj.value("C").toDouble()) + " | Жиры: \t" + QString::number(jsonObj.value("F").toDouble())+
            " | Белки: \t" + QString::number(jsonObj.value("P").toDouble());
    btn->setText(jsonObj.value("N").toString() + "\n" + text);
    btn->setStatusTip(product);
    btn->setMinimumHeight(35);
    btn->setMinimumWidth(330);
    if((jsonObj.value("N").toString().length()*28) > (text.length()*28)) btn->setMaximumWidth(jsonObj.value("N").toString().length()*28);
    else btn->setMaximumWidth(text.length()*28);

    CheckButtons.append(btn);

    for (int i = 0; i < CheckButtons.size(); ++i){
        controlGroups->layout()->addWidget(CheckButtons.at(i));
    }
    ui->scrollArea_2->setWidget(controlGroups);
    ui->scrollArea_2->setWidgetResizable(true);
}

void adder::GetProduct(const QString &DataType, const QJsonArray &data)
{
    type = DataType;

    QWidget *controlGroups = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSizeConstraint(QLayout::SetMinAndMaxSize);
    controlGroups->setLayout(layout);

    for (int i = 0; i< data.size(); ++i){
        QCheckBox* btn = new QCheckBox(this);
        QString text = "Каллории: \t" + QString::number(data.at(i).toObject().value("E").toDouble()) +
                " | Углеводы: \t" + QString::number(data.at(i).toObject().value("C").toDouble()) + " | Жиры: \t" + QString::number(data.at(i).toObject().value("F").toDouble())+
                " | Белки: \t" + QString::number(data.at(i).toObject().value("P").toDouble());
        btn->setText(data.at(i).toObject().value("N").toString() + "\n" + text);
        btn->setStatusTip(QJsonDocument(data.at(i).toObject()).toJson(QJsonDocument::Compact));
        btn->setMinimumHeight(35);
        if((data.at(i).toObject().value("N").toString().length()*28) > (text.length()*28)) btn->setMaximumWidth(data.at(i).toObject().value("N").toString().length()*28);
        else btn->setMaximumWidth(text.length()*28);

        CheckButtons.append(btn);
    }
    for (int i = 0; i < CheckButtons.size(); ++i){
        controlGroups->layout()->addWidget(CheckButtons.at(i));
    }
    ui->scrollArea_2->setWidget(controlGroups);
    ui->scrollArea_2->setWidgetResizable(true);

}

void adder::on_FindButton_clicked()
{
    this->close();
    finder->show();
}

void adder::on_DeleteButton_clicked()
{
    for (int i = 0; i < CheckButtons.size(); ++i){
        if (CheckButtons.at(i)->isChecked()){
            CheckButtons.remove(i);
        }
    }
    QWidget *controlGroups = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSizeConstraint(QLayout::SetMinAndMaxSize);
    controlGroups->setLayout(layout);

    for (int i = 0; i < CheckButtons.size(); ++i){
        controlGroups->layout()->addWidget(CheckButtons.at(i));
    }
    ui->scrollArea_2->setWidget(controlGroups);
}

void adder::on_SaveButton_clicked()
{
    QJsonObject ToSend;
    QJsonArray Array;
    for (int i = 0; i < CheckButtons.size(); ++i){
        Array.append(QJsonDocument::fromJson(CheckButtons.at(i)->statusTip().toUtf8()).object());
    }
    ToSend.insert("array", Array);
    ToSend.insert("type", type);

    emit SendData(ToSend);

    CheckButtons.clear();
    type.clear();
    ui->scrollArea_2->widget()->close();
    this->close();
}

void adder::on_BackButton_clicked()
{
    emit Back();

    CheckButtons.clear();
    type.clear();
    ui->scrollArea_2->widget()->close();
    this->close();
}

void adder::on_favButton_clicked()
{
    if (CheckButtons.size() > 0){
        bool bOk;
        QString str = QInputDialog::getText(this,
                                             "Избранное",
                                             "Введите название для рациона:",
                                             QLineEdit::Normal,
                                             "",
                                             &bOk
                                            );
        if (bOk) {
            if (str.length() > 25) QMessageBox::critical(this, "Ошибка!", "Слишком длинное название!");
            else if (str.length() == 0) QMessageBox::critical(this, "Ошибка!", "Слишком коротное название!");
            else{
                QJsonObject ToSend;
                QJsonArray Array;
                for (int i = 0; i < CheckButtons.size(); ++i){
                    Array.append(QJsonDocument::fromJson(CheckButtons.at(i)->statusTip().toUtf8()).object());
                }
                ToSend.insert("array", Array);
                ToSend.insert("type", type);

                emit SendFavorite(ToSend, str);

                QMessageBox::information(this, "Избранное", "Рацион успешно добавлен в избранные!");
            }
        }
    }
    else{
        QMessageBox::critical(this, "Ошибка!", "В рационе нет продуктов!");
    }
}
