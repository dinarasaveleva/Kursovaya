#ifndef OSNOVA_H
#define OSNOVA_H

#include <QWidget>
#include <QTabWidget>
#include <adder.h>
#include <obmen.h>
#include "favoritebtns.h"


namespace Ui {
class osnova;
}

class osnova : public QWidget
{
    Q_OBJECT

public:
    explicit osnova(QWidget *parent = nullptr);
    ~osnova();

public slots:
    void showInterface(const QString &login, const QString &access_id);
    void GetFirstData();
private slots:
    void GetData(const QJsonObject &data);
    void GetDataFromServer(const QJsonObject &data);
    void SendFavorite(const QJsonObject &data, const QString &name);
    void Error(const QString &info);
    void dateChanged();
    void DelFavorite(const QString &name);

    void connectionError();

    void getfavoriteOk(const QJsonArray &info);

    void getproductOk(const QString &info);

    void on_CalendarButton_clicked();
    void on_ZavtrakButton_clicked();
    void on_PerekusButton_clicked();
    void on_ObedButton_clicked();
    void on_PoldnikButton_clicked();
    void on_UginButton_clicked();

    void on_IzbrannoeButton_clicked();

signals:
    void ToProducts(const QString &DataType, const QJsonArray &data);
    void ToFavorite(const QJsonArray &data);
private:
    QJsonObject allData;
    Ui::osnova *ui;
    obmen *http;
    adder *add;
    FavoriteBtns *fav;
    QString Access_ID;
    QString Login;

};

#endif // OSNOVA_H
