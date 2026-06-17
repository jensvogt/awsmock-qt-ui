//
// Created by vogje01 on 11/23/25.
//

#pragma once

// C++ includes
#include <utility>
#include <iostream>

// Qt includes
#include <QObject>
#include <QUrlQuery>
#include <QRegularExpression>
#include <QtWebSockets/QWebSocket>

// Awsmock includes
#include <utils/Logging.h>

class DockerLogClient final : public QObject {
    Q_OBJECT

public:
    DockerLogClient(QString containerId, QString endpoint, long limit, QObject *parent);

    void ConnectToDocker() const;

    void DisconnectFromDocker() const;

signals:
    void LogReceived(const QStringList &lines);

    void Connected();

    void Disconnected();

    void ErrorOccurred(const QString &msg);

private slots:
    void onConnected();

    void onTextMessageReceived(const QString &message);

    void onDisconnected();

    void onErrorOccurred(QAbstractSocket::SocketError error);

    static QStringList SanitizeString(const QString &input);

private:
    QString m_containerId;
    QString m_endpoint;

    QWebSocket *m_webSocket = nullptr;

    long _limit = 1000;
    bool m_hasError = false;
};
