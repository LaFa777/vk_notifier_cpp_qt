#include "vknotifer.h"

// @todo добавить звук // пункт в меню для звука

VkNotifer::VkNotifer()
{
    tray     = new QSystemTrayIcon();
    settings = new QSettings();
    network  = new QNetworkAccessManager();

    interval = 2000;

    // initialization tray menu
    trayMenu = new QMenu();
        trayMenu->addAction(tr("выход"),QApplication::instance(),SLOT(quit()));

    tray->setContextMenu(trayMenu);

    tray->show();

    // prepare url
    QUrlQuery* query = new QUrlQuery();
        query->addQueryItem("user_id", settings->value("app/userId").toString() );
        query->addQueryItem("v","5.5");
        query->addQueryItem("access_token",settings->value("app/accessToken").toString());

    urlCheckMessage = new QUrl();
        urlCheckMessage->setScheme("https");
        urlCheckMessage->setHost("api.vk.com");
        urlCheckMessage->setPath("/method/execute.checkMessage");
        urlCheckMessage->setQuery(*query);

    requestCheckMessage = new QNetworkRequest(*urlCheckMessage);

    network = new QNetworkAccessManager();
        connect(network,SIGNAL(finished(QNetworkReply*)),SLOT(update(QNetworkReply*)));

    timer = new QTimer();
    connect(timer,SIGNAL(timeout()),SLOT(start()));
    timer->setInterval(interval);
    timer->start();

    connect(this,SIGNAL(notify()),SLOT(sound()));
}
void VkNotifer::start()
{
    qDebug() << "start()";

    qint64 tokenTime = settings->value("app/secTimeAuth","0").toInt();
    qint64 currentTime = QDateTime::currentMSecsSinceEpoch() / 1000; // get current sec 1970

    if ( tokenTime == 0 || currentTime >= tokenTime ){
        this->authentication();
    }else{
        checkMessage();
    }
}

void VkNotifer::authentication(){
    qDebug() << "authentication()";

    QUrl* authUrl = new QUrl("https://oauth.vk.com/authorize?client_id=3635428&scope=4096&redirect_uri=https://oauth.vk.com/blank.html&display=mobile&v=5.5&response_type=token");

    browser = new QWebView();
        browser->resize(300,500);
        browser->setWindowFlags(Qt::Dialog);
        browser->setUrl(*authUrl);
        browser->show();

    connect(browser,SIGNAL(urlChanged(QUrl)),SLOT(authenticationFinished(QUrl)));
}
void VkNotifer::authenticationFinished(QUrl url){
    qDebug() << "authenticationFinished()";

    if ( url.toString().startsWith("https://oauth.vk.com/blank.html") )
    {
        QUrlQuery* query = new QUrlQuery( url.fragment() );

        settings->setValue( "app/accessToken", query->queryItemValue("access_token") );

        qint64 currentTime = QDateTime::currentMSecsSinceEpoch() / 1000;
        settings->setValue( "app/secTimeAuth", currentTime + query->queryItemValue("expires_in").toInt() );

        settings->setValue( "app/userId", query->queryItemValue("user_id") );

        // browser->hide();
        delete browser;
    }
}

void VkNotifer::checkMessage()
{
    qDebug() << "checkMessage()";
    network->get(*requestCheckMessage);
}
void VkNotifer::update(QNetworkReply *reply)
{
    qDebug() << "update()";

    int currentCoutMess = QJsonDocument::fromJson( reply->readAll() ).object()["response"].toDouble();

    qDebug() << currentCoutMess;

    if ( currentCoutMess == 1 && currentCoutMess != coutMess ){
        tray->showMessage(tr("Новое сообщение"),tr("У тебя новое сообщение..."));
        emit notify();
    }else if( currentCoutMess > 1 && currentCoutMess != coutMess ){
        tray->showMessage(
                    tr("Новые сообщения"),
                    tr("У тебя ") + QString::number(currentCoutMess) + tr(" новых сообщений")
                    );
        emit notify();
    }
    coutMess = currentCoutMess;
}
void VkNotifer::sound()
{
    qDebug() << "sound()";
    //QSound::play("./beep.wav");
    QSound::play(":/music/beep.wav");
}
