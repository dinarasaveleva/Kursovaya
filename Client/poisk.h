#ifndef POISK_H
#define POISK_H

#include <QWidget>
#include <obmen.h>
#include <QRadioButton>
#include <QMessageBox>
#include <QValidator>

namespace Ui {
class Poisk;
}

class Poisk : public QWidget
{
    Q_OBJECT

public:
    explicit Poisk(QWidget *parent = nullptr);
    ~Poisk();

private slots:
    void on_FindButton_clicked();

    void on_AddButton_clicked();

    void on_CloseButton_3_clicked();

signals:
    void AddProduct(const QString &product);
    void Close();

private:
    void DrawButtons();
    QVector<QRadioButton*> radioButtons;
    Ui::Poisk *ui;
    obmen *http;
};

#endif // POISK_H
