#include "osnova.h"
#include "ui_osnova.h"

osnova::osnova(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::osnova)
{
    ui->setupUi(this);
    add = new adder;
    http = new obmen;
    fav = new FavoriteBtns;

    ui->CalendarButton->setText(QDate::currentDate().toString());
    ui->calendarWidget->hide();

    connect(ui->calendarWidget, &QCalendarWidget::selectionChanged, this, &osnova::dateChanged);
    connect(http, SIGNAL(addproductError(QString)), this, SLOT(Error(QString)));
    connect(http, SIGNAL(addfavoriteError(QString)), this, SLOT(Error(QString)));
    connect(http, SIGNAL(getproductOk(QString)), this, SLOT(getproductOk(QString)));
    connect(http, SIGNAL(getproductError(QString)), this, SLOT(Error(QString)));
    connect(http, SIGNAL(getfavoriteOk(QJsonArray)), this, SLOT(getfavoriteOk(QJsonArray)));
    connect(http, SIGNAL(getfavoriteError(QString)), this, SLOT(Error(QString)));
    connect(http, SIGNAL(delfavoriteError(QString)), this, SLOT(Error(QString)));
    connect(http, SIGNAL(connectionError()), this, SLOT(connectionError()));
    connect(this, SIGNAL(ToProducts(QString,QJsonArray)), add, SLOT(GetProduct(QString,QJsonArray)));
    connect(add, &adder::SendData, this, &osnova::show);
    connect(add, &adder::Back, this, &osnova::show);
    connect(add, SIGNAL(SendData(QJsonObject)), this, SLOT(GetData(QJsonObject)));
    connect(add, SIGNAL(SendFavorite(QJsonObject,QString)), this, SLOT(SendFavorite(QJsonObject,QString)));
    connect(this, SIGNAL(ToFavorite(QJsonArray)), fav, SLOT(GetFavorite(QJsonArray)));
    connect(fav, &FavoriteBtns::Back, this, &osnova::show);
    connect(fav, SIGNAL(ToMain(QJsonObject)), this, SLOT(GetData(QJsonObject)));
    connect(fav, SIGNAL(DelToMain(QString)), this, SLOT(DelFavorite(QString)));
}

osnova::~osnova()
{
    delete ui;
}

void osnova::GetFirstData(){
    http->getproduct(Login, Access_ID, ui->calendarWidget->selectedDate().toString("dd-MM-yyyy"));
}

void osnova::showInterface(const QString &login, const QString &access_id){
    Access_ID = access_id;
    Login = login;
}

void osnova::Error(const QString &info){
    QMessageBox::critical(this, "Ошибка!", info);
}

void osnova::dateChanged()
{
    ui->CalendarButton->setText(ui->calendarWidget->selectedDate().toString());
    allData = QJsonObject();
    ui->CaloriesNumText->setText("0");

    http->getproduct(Login, Access_ID, ui->calendarWidget->selectedDate().toString("dd-MM-yyyy"));
}

void osnova::DelFavorite(const QString &name)
{
    http->delfavorite(name, Access_ID, Login);
}

void osnova::connectionError()
{
    QMessageBox::critical(this, "Ошибка!", "Ошибка подключения! Проверте подключение к интернету или подождите несколько секунд и повторите попытку.");
}

void osnova::getfavoriteOk(const QJsonArray &info)
{
    emit ToFavorite(info);

    this->close();
    fav->show();
}

void osnova::getproductOk(const QString &info){
    QJsonParseError parseError;
    const QJsonDocument jsonDoc = QJsonDocument::fromJson(info.toUtf8(), &parseError);
    if (parseError.error == QJsonParseError::NoError) {
        if (jsonDoc.isObject()){
            GetDataFromServer(jsonDoc.object());
        }
    }
}

void osnova::GetData(const QJsonObject &data){
    double Calories = 0;
    allData.insert(data.value("type").toString(), data.value("array").toArray());

    if (!allData.value("Breakfast").isNull() && allData.value("Breakfast").isArray()){
        QJsonArray prods = allData.value("Breakfast").toArray();
        for (int i = 0; i < prods.size(); ++i){
            Calories += prods.at(i).toObject().value("C").toDouble();
        }
    }
    if (!allData.value("Snack").isNull() && allData.value("Snack").isArray()){
        QJsonArray prods = allData.value("Snack").toArray();
        for (int i = 0; i < prods.size(); ++i){
            Calories += prods.at(i).toObject().value("C").toDouble();
        }
    }
    if (!allData.value("Lunch").isNull() && allData.value("Lunch").isArray()){
        QJsonArray prods = allData.value("Lunch").toArray();
        for (int i = 0; i < prods.size(); ++i){
            Calories += prods.at(i).toObject().value("C").toDouble();
        }
    }
    if (!allData.value("AfternoonSnack").isNull() && allData.value("AfternoonSnack").isArray()){
        QJsonArray prods = allData.value("AfternoonSnack").toArray();
        for (int i = 0; i < prods.size(); ++i){
            Calories += prods.at(i).toObject().value("C").toDouble();
        }
    }
    if (!allData.value("Supper").isNull() && allData.value("Supper").isArray()){
        QJsonArray prods = allData.value("Supper").toArray();
        for (int i = 0; i < prods.size(); ++i){
            Calories += prods.at(i).toObject().value("C").toDouble();
        }
    }

    http->addproduct(Login, Access_ID, ui->calendarWidget->selectedDate().toString("dd-MM-yyyy"), QJsonDocument(allData).toJson(QJsonDocument::Compact));

    ui->CaloriesNumText->setText(QString::number(Calories));
}

void osnova::GetDataFromServer(const QJsonObject &data)
{
    double Calories = 0;
    if (!data.value("Breakfast").isNull() && data.value("Breakfast").isArray()){
        QJsonArray prods = data.value("Breakfast").toArray();
        for (int i = 0; i < prods.size(); ++i){
            Calories += prods.at(i).toObject().value("C").toDouble();
        }
        allData.insert("Breakfast", data.value("Breakfast").toArray());
    }
    if (!data.value("Snack").isNull() && data.value("Snack").isArray()){
        QJsonArray prods = data.value("Snack").toArray();
        for (int i = 0; i < prods.size(); ++i){
            Calories += prods.at(i).toObject().value("C").toDouble();
        }
        allData.insert("Snack", data.value("Snack").toArray());
    }
    if (!data.value("Lunch").isNull() && data.value("Lunch").isArray()){
        QJsonArray prods = data.value("Lunch").toArray();
        for (int i = 0; i < prods.size(); ++i){
            Calories += prods.at(i).toObject().value("C").toDouble();
        }
        allData.insert("Lunch", data.value("Lunch").toArray());
    }
    if (!data.value("AfternoonSnack").isNull() && data.value("AfternoonSnack").isArray()){
        QJsonArray prods = data.value("AfternoonSnack").toArray();
        for (int i = 0; i < prods.size(); ++i){
            Calories += prods.at(i).toObject().value("C").toDouble();
        }
        allData.insert("AfternoonSnack", data.value("AfternoonSnack").toArray());
    }
    if (!data.value("Supper").isNull() && data.value("Supper").isArray()){
        QJsonArray prods = data.value("Supper").toArray();
        for (int i = 0; i < prods.size(); ++i){
            Calories += prods.at(i).toObject().value("C").toDouble();
        }
        allData.insert("Supper", data.value("Supper").toArray());
    }

    ui->CaloriesNumText->setText(QString::number(Calories));
}

void osnova::SendFavorite(const QJsonObject &data, const QString &name)
{
    http->addfavorite(Login, Access_ID, name, QJsonDocument(data).toJson(QJsonDocument::Compact));
}

void osnova::on_CalendarButton_clicked()
{
    if(ui->calendarWidget->isHidden()) ui->calendarWidget->show();
    else ui->calendarWidget->hide();
}

void osnova::on_ZavtrakButton_clicked()
{
    emit ToProducts("Breakfast", allData.value("Breakfast").toArray());
    this->close();
    add->show();
}

void osnova::on_PerekusButton_clicked()
{
    emit ToProducts("Snack", allData.value("Snack").toArray());
    this->close();
    add->show();
}

void osnova::on_ObedButton_clicked()
{
    emit ToProducts("Lunch", allData.value("Lunch").toArray());
    this->close();
    add->show();
}

void osnova::on_PoldnikButton_clicked()
{
    emit ToProducts("AfternoonSnack", allData.value("AfternoonSnack").toArray());
    this->close();
    add->show();
}

void osnova::on_UginButton_clicked()
{
    emit ToProducts("Supper", allData.value("Supper").toArray());
    this->close();
    add->show();
}

void osnova::on_IzbrannoeButton_clicked()
{
    http->getfavorite(Login, Access_ID);

}
