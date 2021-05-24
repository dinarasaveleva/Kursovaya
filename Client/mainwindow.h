#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include "registr.h"
#include "osnova.h"
#include "obmen.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void authentificationOk(const QString &access_id);
    void authentificationError(const QString &info);
    void connectionError();

    void on_AuthButton_clicked();
    void on_RegistrButton_clicked();

private:
    Ui::MainWindow *ui;
    registr *reg;
    osnova *interface;
    obmen *http;
    QString login;
signals:
    void showInterface(const QString &login, const QString &access_id);
};
#endif // MAINWINDOW_H
