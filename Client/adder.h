#ifndef ADDER_H
#define ADDER_H

#include <QWidget>
#include <QCheckBox>
#include <QInputDialog>
#include <poisk.h>

namespace Ui {
class adder;
}

class adder : public QWidget
{
    Q_OBJECT

public:
    explicit adder(QWidget *parent = nullptr);
    ~adder();

private slots:
    void GetProduct(const QString &product);
    void GetProduct(const QString &DataType, const QJsonArray &data);

    void on_FindButton_clicked();
    void on_DeleteButton_clicked();
    void on_SaveButton_clicked();
    void on_BackButton_clicked();

    void on_favButton_clicked();

signals:
    void SendData(const QJsonObject &data);
    void SendFavorite(const QJsonObject &data, const QString &name);

    void Back();
private:
    QString type;
    Ui::adder *ui;
    QVector<QCheckBox*> CheckButtons;
    Poisk *finder;
};

#endif // ADDER_H
