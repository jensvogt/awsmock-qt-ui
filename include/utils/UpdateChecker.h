//
// Created by vogje01 on 2/8/26.
//

#ifndef AWSMOCK_QT_UI_UPDATE_CHECKER_H
#define AWSMOCK_QT_UI_UPDATE_CHECKER_H

// Qt includes
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QCoreApplication>
#include <QVersionNumber>

// Awsmock includes
#include <Version.h>

class UpdateChecker : public QObject {
    Q_OBJECT

public:
    UpdateChecker(QObject *parent = nullptr) : QObject(parent) {
    }

    void checkForUpdates() {
        const QUrl url("https://jensvogt.github.io/awsmock-qt-ui/version.txt");
        const QNetworkRequest request(url);

        QNetworkReply *reply = manager.get(request);
        connect(reply, &QNetworkReply::finished, this, [this, reply]() {
            QString latest = reply->errorString();
            logInfo << reply->errorString();
            if (reply->error() == QNetworkReply::NoError) {
                const QString latestVersion = QString(reply->readAll()).trimmed();
                compareVersions(latestVersion);
            }
            reply->deleteLater();
        });
    }

private:
    void compareVersions(const QString &latest) {
        QVersionNumber currentV = QVersionNumber::fromString(APP_VERSION);
        QVersionNumber latestV = QVersionNumber::fromString(latest);

        if (latest > currentVersion) {
            qInfo() << "Update Available! Current:" << currentVersion << "Latest:" << latest;
            emit UpdateAvailable(latest);
        } else {
            qInfo() << "You are up to date.";
        }
    }

    QNetworkAccessManager manager;
    QString currentVersion;

signals:
    void UpdateAvailable(const QString &ver);
};
#endif //AWSMOCK_QT_UI_UPDATE_CHECKER_H
