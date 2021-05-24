#include "registr.h"
#include "ui_registr.h"

registr::registr(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::registr)
{
    ui->setupUi(this);
    http = new obmen;

    ui->LoginEdit->setValidator(new QRegExpValidator(QRegExp("[А-яA-z0-9]{10}")));

    connect(http, SIGNAL(registrationOk()), this, SLOT(registrationOk()));
    connect(http, SIGNAL(registrationError(QString)), this, SLOT(registrationError(QString)));

}

registr::~registr()
{
    delete ui;
}

void registr::registrationOk()
{
    QMessageBox::information(this, "Регистрация", "Вы успешно зарегестрированны!");
    emit showAuth();
    this->close();
}

void registr::registrationError(const QString &info)
{
    QMessageBox::critical(this, "Ошибка", info);
}

void registr::on_RegistrButton_clicked()
{
    if (ui->PasswordEdit->text() == ui->ConfPasswordEdit->text()){
        if (ui->LoginEdit->text().length() == 0 && ui->PasswordEdit->text().length() == 0)
        {
            QMessageBox::critical(this, "Ошибка", "Логин или пароль пуст!");
        }
        else{
            http->registration(ui->LoginEdit->text(), ui->PasswordEdit->text());
        }
    }
    else{
        QMessageBox::critical(this, "Ошибка", "Указанные пароли не совпадают!");
    }
}
