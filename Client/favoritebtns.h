#ifndef FAVORITEBTNS_H
#define FAVORITEBTNS_H

#include <QWidget>
#include <QPushButton>
#include "obmen.h"
#include "favorite.h"

namespace Ui {
class FavoriteBtns;
}

class FavoriteBtns : public QWidget
{
    Q_OBJECT

public:
    explicit FavoriteBtns(QWidget *parent = nullptr);
    ~FavoriteBtns();
public slots:
    void GetFavorite(const QJsonArray &info);
private slots:
    void onClicked();
    void Save(const QJsonObject &data);
    void Del(const QString &name);
    void on_BackButton_clicked();
signals:
    void ToMain(const QJsonObject &data);
    void DelToMain(const QString &name);
    void ToFavorites(const QJsonObject &data);
    void Back();
private:
    obmen *http;
    QVector<QPushButton*> PushButtons;
    Favorite *fav;
    Ui::FavoriteBtns *ui;
};

#endif // FAVORITEBTNS_H
