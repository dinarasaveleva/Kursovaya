#include "favoritebtns.h"
#include "ui_favoritebtns.h"

FavoriteBtns::FavoriteBtns(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FavoriteBtns)
{
    ui->setupUi(this);
    http = new obmen;
    fav = new Favorite;

    connect(this, SIGNAL(ToFavorites(QJsonObject)), fav, SLOT(ToFavorites(QJsonObject)));
    connect(fav, &Favorite::Back, this, &FavoriteBtns::show);
    connect(fav, &Favorite::Del, this, &FavoriteBtns::show);
    connect(fav, &Favorite::Save, this, &FavoriteBtns::show);
    connect(fav, SIGNAL(Save(QJsonObject)), this, SLOT(Save(QJsonObject)));
    connect(fav, SIGNAL(Del(QString)), this, SLOT(Del(QString)));


}

FavoriteBtns::~FavoriteBtns()
{
    delete ui;
}

void FavoriteBtns::GetFavorite(const QJsonArray &info){
    QWidget *controlGroups = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSizeConstraint(QLayout::SetMinAndMaxSize);
    controlGroups->setLayout(layout);

    for (int i = 0; i< info.size(); ++i){
        QPushButton* btn = new QPushButton(this);
        btn->setText(info.at(i).toObject().value("name").toString());
        btn->setStatusTip(QJsonDocument(info.at(i).toObject()).toJson(QJsonDocument::Compact));
        btn->setMinimumHeight(35);
        btn->setMinimumWidth(info.at(i).toObject().value("name").toString().length()*28);
        connect(btn, &QPushButton::clicked, this, &FavoriteBtns::onClicked);

        PushButtons.append(btn);
    }
    for (int i = 0; i < PushButtons.size(); ++i){
        controlGroups->layout()->addWidget(PushButtons.at(i));
    }
    ui->scrollArea->setWidget(controlGroups);
    ui->scrollArea->setWidgetResizable(true);
}

void FavoriteBtns::onClicked()
{
    QPushButton *b = qobject_cast<QPushButton*>(sender());

    QJsonParseError parseError;
    const QJsonDocument jsonDoc = QJsonDocument::fromJson(b->statusTip().toUtf8(), &parseError);
    if (parseError.error == QJsonParseError::NoError) {
        if (jsonDoc.isObject()){
            emit ToFavorites(jsonDoc.object());

            this->close();
            fav->show();
        }
    }

}

void FavoriteBtns::Save(const QJsonObject &data)
{
    on_BackButton_clicked();

    emit ToMain(data);
}

void FavoriteBtns::Del(const QString &name)
{
    on_BackButton_clicked();

    emit DelToMain(name);
}

void FavoriteBtns::on_BackButton_clicked()
{
    this->close();
    ui->scrollArea->widget()->close();
    PushButtons.clear();

    emit Back();
}
