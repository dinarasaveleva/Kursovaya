#ifndef REGISTR_H
#define REGISTR_H

#include <QWidget>
#include <QValidator>
#include <QMessageBox>
#include <obmen.h>

namespace Ui {
class registr;
}

class registr : public QWidget
{
    Q_OBJECT

public:
    explicit registr(QWidget *parent = nullptr);
    ~registr();

private slots:
    void registrationOk();
    void registrationError(const QString &info);

    void on_RegistrButton_clicked();

private:
    Ui::registr *ui;
    obmen *http;
signals:
    void showAuth();
    void sendReg(const QString &login, const QString &password);
};

#endif // REGISTR_H
