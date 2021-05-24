#ifndef OBMEN_H
#define OBMEN_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QNetworkRequest>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QTimer>
#include <QCryptographicHash>

class obmen : public QObject
{
    Q_OBJECT
public:
    explicit obmen(QObject *parent = nullptr);
    ~obmen();
    QJsonArray Info;

public slots:
    void sendGetData(const QUrl &url, const QString &contentType);
    void sendPostData(const QNetworkRequest &request, const QByteArray &param);
    void getResponse(QNetworkReply *reply);
    void search(const QString &data);
    void authentification(const QString &login, const QString &password);
    void registration(const QString &login, const QString &password);
    void addproduct(const QString &login, const QString &access_id, const QString &date, const QString &products);
    void getproduct(const QString &login, const QString &access_id, const QString &date);
    void addfavorite(const QString &login, const QString &access_id, const QString &name, const QString &products);
    void getfavorite(const QString &login, const QString &access_id);
    void delfavorite(const QString &name, const QString &access_id, const QString &login);

private slots:
    void translateInfo(const QString &data);
private:
    QNetworkAccessManager *mngr;
    void findApiInfo(QJsonObject &data);
    void findServerInfo(QJsonObject &data);

    bool Translated = false;
signals:
    void ready();
    void connectionError();

    void authentificationOk(const QString &access_id);
    void authentificationError(const QString &info);

    void addfavoriteError(const QString &info);

    void registrationOk();
    void registrationError(const QString &info);

    void addproductError(const QString &info);

    void getfavoriteOk(const QJsonArray &info);
    void getfavoriteError(const QString &info);

    void getproductOk(const QString &info);
    void getproductError(const QString &info);

    void delfavoriteError(const QString &info);
};

#endif // OBMEN_H
