#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->LoginEdit->setValidator(new QRegExpValidator(QRegExp("[А-яA-z0-9]{10}")));

    reg = new registr;
    interface = new osnova;
    http = new obmen;
    connect(this, SIGNAL(showInterface(QString,QString)), interface, SLOT(showInterface(QString,QString)));
    connect(this, &MainWindow::showInterface, interface, &osnova::GetFirstData);
    connect(reg, &registr::showAuth, this, &MainWindow::show);
    connect(http, SIGNAL(authentificationOk(QString)), this, SLOT(authentificationOk(QString)));
    connect(http, SIGNAL(authentificationError(QString)), this, SLOT(authentificationError(QString)));
    connect(http, SIGNAL(connectionError()), this, SLOT(connectionError()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::authentificationOk(const QString &access_id)
{
    emit showInterface(login, access_id);
    interface->show();
    this->close();
}


void MainWindow::authentificationError(const QString &info)
{
    QMessageBox::critical(this, "Ошибка!", info);
}

void MainWindow::connectionError()
{
    QMessageBox::critical(this, "Ошибка!", "Ошибка подключения! Проверте подключение к интернету или подождите несколько секунд и повторите попытку.");
}


void MainWindow::on_AuthButton_clicked()
{
    http->authentification(ui->LoginEdit->text(), ui->PasswordEdit->text());
    login = ui->LoginEdit->text();
}

void MainWindow::on_RegistrButton_clicked()
{
    this->close();
    reg->show();
}
