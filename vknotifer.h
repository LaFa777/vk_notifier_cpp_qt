#ifndef VKNOTIFER_H
#define VKNOTIFER_H

// core
#include <QDebug>
#include <QSettings>
#include <QDateTime>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QApplication>
#include <QTimer>

// widgets
#include <QWebView>
#include <QSystemTrayIcon>
#include <QMenu>

// network
#include <QNetworkAccessManager>
#include <QUrl>
#include <QUrlQuery>
#include <QNetworkReply>
#include <QNetworkRequest>

// multimedia
#include <QSound>

class VkNotifer : public QObject
{
    Q_OBJECT
private:
    QSettings*             settings;
    QWebView*              browser;
    QSystemTrayIcon*       tray;
    QMenu*                 trayMenu;
    QNetworkAccessManager* network;
    QUrl*                  urlCheckMessage;
    QNetworkRequest*       requestCheckMessage;
    QTimer* timer;
    int interval;
    int coutMess;
public:
    VkNotifer();
    void authentication();
    void checkMessage();
public slots:
    void start();
    void authenticationFinished(QUrl url);
    void update(QNetworkReply* reply);
    void sound();
signals:
    void notify();
};

#endif // VKNOTIFER_H
