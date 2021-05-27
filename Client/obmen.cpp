#define SERVER "http://192.168.0.121:5000/"
#include "obmen.h"

obmen::obmen(QObject *parent) : QObject(parent)
{
    mngr = new QNetworkAccessManager(this);
    connect(mngr, SIGNAL(finished(QNetworkReply*)), this, SLOT(getResponse(QNetworkReply*)));

}
obmen::~obmen(){
    delete mngr;
}

void obmen::search(const QString &data){
    QString TranslateUrl = QString("https://fasttranslator.herokuapp.com/api/v1/text/to/text?source=%1&lang=ru-en").arg(data);
    sendGetData(QUrl(TranslateUrl), "text/plain");
}

void obmen::authentification(const QString &login, const QString &password)
{
    QNetworkRequest request;
    request.setUrl(QUrl(QString(SERVER) + "authentification"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QByteArray param;
    param.append("login=" +login.toUtf8());
    param.append("&password=" + QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex());

    sendPostData(request, param);
}

void obmen::registration(const QString &login, const QString &password)
{
    QNetworkRequest request;
    request.setUrl(QUrl(QString(SERVER) + "registration"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QByteArray param;
    param.append("login=" +login.toUtf8());
    param.append("&password=" + QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex());

    sendPostData(request, param);
}

void obmen::addproduct(const QString &login, const QString &access_id, const QString &date, const QString &products)
{
    QNetworkRequest request;
    request.setUrl(QUrl(QString(SERVER) + "addproduct"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QByteArray param;
    param.append("user=" +login.toUtf8());
    param.append("&access_id=" + access_id.toUtf8());
    param.append("&date=" + date.toUtf8());
    param.append("&products=" + products.toUtf8());

    sendPostData(request, param);
}

void obmen::getproduct(const QString &login, const QString &access_id, const QString &date){
    QNetworkRequest request;
    request.setUrl(QUrl(QString(SERVER) + "getproduct"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QByteArray param;
    param.append("user=" +login.toUtf8());
    param.append("&access_id=" + access_id.toUtf8());
    param.append("&date=" + date.toUtf8());

    sendPostData(request, param);

}

void obmen::addfavorite(const QString &login, const QString &access_id, const QString &name, const QString &products)
{
     QNetworkRequest request;
     request.setUrl(QUrl(QString(SERVER) + "addfavorite"));
     request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

     QByteArray param;
     param.append("user=" +login.toUtf8());
     param.append("&access_id=" + access_id.toUtf8());
     param.append("&name=" + name.toUtf8());
     param.append("&products=" + products.toUtf8());

     sendPostData(request, param);

}

void obmen::getfavorite(const QString &login, const QString &access_id)
{
    QNetworkRequest request;
    request.setUrl(QUrl(QString(SERVER) + "getfavorite"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QByteArray param;
    param.append("user=" +login.toUtf8());
    param.append("&access_id=" + access_id.toUtf8());

    sendPostData(request, param);

}

void obmen::delfavorite(const QString &name, const QString &access_id, const QString &login)
{
    QNetworkRequest request;
    request.setUrl(QUrl(QString(SERVER) + "delfavorite"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QByteArray param;
    param.append("name=" + name.toUtf8());
    param.append("&access_id=" + access_id.toUtf8());
    param.append("&user=" + login.toUtf8());

    sendPostData(request, param);
}

void obmen::findApiInfo(QJsonObject &data){

    QString UrlUsda("https://api.nal.usda.gov/fdc/v1/foods/search?query=%1&api_key=u52Nb2bk3vE2wQF991wmeuO9PqDLuJlGYAoHkTY6&pageSize=9&dataType=Foundation,SR Legacy");

    QJsonValue FullTranslateValue = data.value(QLatin1String("translated_json"));
    QJsonValue TranslateValue = data.value(QLatin1String("data"));
    QJsonValue TypeValue = data.value(QLatin1String("type"));

    if (!TranslateValue.isNull() && TranslateValue.isString() && Translated == false){
        sendGetData(QUrl(UrlUsda.arg(TranslateValue.toString())), "application/json");
        Translated = true;
    }
    else if (Translated == true){
        QJsonArray ArrayInfo;
        QJsonArray foods;
        foods = data.value("foods").toArray();
        for(int i = 0; i < foods.size(); ++i){
            QJsonObject object = foods[i].toObject();
            QJsonArray foodNutrients = object.value("foodNutrients").toArray();
            QJsonObject Info;
            Info.insert("N", object.value("description"));
            QJsonValue caloriesPlanB;
            for (int j = 0; j < foodNutrients.size(); ++j){
                QJsonObject object2 = foodNutrients[j].toObject();
                int ready = 0;
                if (object2.value("nutrientId").toDouble() == 1003){
                    Info.insert("P", object2.value("value"));
                    ++ready;
                }
                if (object2.value("nutrientId").toDouble() == 1005){
                    Info.insert("C", object2.value("value"));
                    ++ready;
                }
                if (object2.value("nutrientId").toDouble() == 1004){
                    Info.insert("F", object2.value("value"));
                    ++ready;
                }
                if (object2.value("nutrientId").toDouble() == 1008){
                    Info.insert("E", object2.value("value"));
                    ++ready;
                }
                if (object2.value("nutrientId").toDouble() == 2047){
                    caloriesPlanB = object2.value("value");
                }

                if (ready == 4) break;
            }
            if (Info.value("E").isUndefined()) Info.insert("E", caloriesPlanB);
            ArrayInfo.append(Info);
            Info = QJsonObject();
        }
        QJsonObject paste;
        paste.insert("text", ArrayInfo);
        translateInfo(QJsonDocument(paste).toJson(QJsonDocument::Compact));
        Translated = false;
    }
    if(!FullTranslateValue.isNull() && FullTranslateValue.isObject()){
       QJsonObject translateVal = FullTranslateValue.toObject();
       QJsonDocument translatedDoc = QJsonDocument::fromJson(translateVal.value("ru").toString().toUtf8());
       Info = translatedDoc.object().value("text").toArray();
       emit ready();
    }


}

void obmen::findServerInfo(QJsonObject &data){
    QJsonValue TypeValue = data.value(QLatin1String("type"));

    if (TypeValue.toString() == "authentification_info"){
        if (data.value(QLatin1String("success")).toBool()){
            emit authentificationOk(data.value(QLatin1String("access_id")).toString());
        }
        else{
            emit authentificationError(data.value(QLatin1String("info")).toString());
        }
    }
    else if (TypeValue.toString() == "registration_info"){
        if (data.value(QLatin1String("success")).toBool()){
            emit registrationOk();
        }
        else{
            emit registrationError(data.value(QLatin1String("info")).toString());
        }
    }
    else if (TypeValue.toString() == "addproduct_info"){
        if (!data.value(QLatin1String("success")).toBool()){
            emit addproductError(data.value(QLatin1String("info")).toString());
        }
    }
    else if (TypeValue.toString() == "getproduct_info"){
        if (data.value(QLatin1String("success")).toBool()){
            emit getproductOk(data.value(QLatin1String("info")).toString());
        }
        else{
            if(!data.value(QLatin1String("info")).isNull() && data.value(QLatin1String("info")).isString()){
                emit getproductError(data.value(QLatin1String("info")).toString());
            }
        }
    }
    else if (TypeValue.toString() == "addfavorite_info"){
        if (!data.value(QLatin1String("success")).toBool()){
            emit addfavoriteError(data.value(QLatin1String("info")).toString());
        }
    }
    else if (TypeValue.toString() == "getfavorite_info"){
        if (data.value(QLatin1String("success")).toBool()){
            emit getfavoriteOk(data.value(QLatin1String("info")).toArray());
        }
        else{
            if(!data.value(QLatin1String("info")).isNull() && data.value(QLatin1String("info")).isString()){
                emit getproductError(data.value(QLatin1String("info")).toString());
            }
        }
    }
    else if (TypeValue.toString() == "delfavorite_info"){
        if (!data.value(QLatin1String("success")).toBool()){
            emit delfavoriteError(data.value(QLatin1String("info")).toString());
        }
    }

}

void obmen::translateInfo(const QString &data){
    QByteArray param;
    param.append("from=en");
    param.append("&to=ru");
    param.append("&json=" + QUrl::toPercentEncoding(data.toUtf8()));
    param.append("&protected_keys=" + QUrl::toPercentEncoding("C;E;F;P"));
    QString TranslateUrl = QString("https://nlp-translation.p.rapidapi.com/v1/jsontranslate");

    QNetworkRequest request;
    request.setUrl(QUrl(TranslateUrl));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setRawHeader("x-rapidapi-key", "ac97a5cc73mshee46f67e435a122p167ed9jsn9eb1015c70fa");
    request.setRawHeader("x-rapidapi-host", "nlp-translation.p.rapidapi.com");
    request.setRawHeader("useQueryString", "true");

    sendPostData(request, param);
}

void obmen::sendGetData(const QUrl &url, const QString &contentType) {
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, contentType);
    mngr->get(request);
}

void obmen::sendPostData(const QNetworkRequest &request, const QByteArray &param) {
    mngr->post(request, param);
}

void obmen::getResponse(QNetworkReply *reply)
{
    QByteArray data = reply->readAll();
    if (reply->error() == QNetworkReply::NoError){
        QJsonParseError parseError;
        QJsonObject Obj;
        const QJsonDocument jsonDoc = QJsonDocument::fromJson(data, &parseError);
        if (parseError.error == QJsonParseError::NoError) {
            if (jsonDoc.isObject()){
                Obj = jsonDoc.object();
            }
        }
        QJsonValue TypeValue = Obj.value(QLatin1String("type"));

        if (TypeValue.isNull() || !TypeValue.isString()){
            findApiInfo(Obj);
        }
        else{
            findServerInfo(Obj);
        }
    }
    else{
        emit connectionError();
    }
}
