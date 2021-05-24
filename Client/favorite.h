#ifndef FAVORITE_H
#define FAVORITE_H

#include <QWidget>
#include <QJsonDocument>
#include <QJsonObject>
#include <QCheckBox>
#include <QJsonArray>

namespace Ui {
class Favorite;
}

class Favorite : public QWidget
{
    Q_OBJECT

public:
    explicit Favorite(QWidget *parent = nullptr);
    ~Favorite();
public slots:
    void ToFavorites(const QJsonObject &data);
signals:
    void Back();
    void Save(const QJsonObject &data);
    void Del(const QString &name);
private slots:
    void on_BackButton_clicked();

    void on_SaveButton_clicked();
    void on_DelButton_clicked();

private:
    Ui::Favorite *ui;
    QVector<QCheckBox*> CheckButtons;
    QJsonObject DataToMain;
    QString Name;
};

#endif // FAVORITE_H
