//
// Created by vogje01 on 11/23/25.
//

#ifndef AWSMOCK_QT_UI_DOCKER_LOG_CLIENT_H
#define AWSMOCK_QT_UI_DOCKER_LOG_CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QLocalSocket>
#include <QDebug>
#include <QRegularExpression>

class DockerLogClient final : public QObject {
    Q_OBJECT

public:
    enum class Mode {
        UnixSocket,
        Tcp
    };

    DockerLogClient(const QString &containerId, Mode mode, const QString &endpoint, QObject *parent = nullptr);

    void ConnectToDocker() const;

    void DisconnectFromDocker() const;

signals:
    void LogReceived(const QStringList &line);

    void Connected();

    void Disconnected();

    void ErrorOccurred(const QString &msg);

private slots:
    void onConnected();

    void onReadyRead();

    void onErrorOccurred();

    static QStringList SanitizeString(const QString &input);

private:
    void SendRequest() const;

    QString m_containerId;
    QString m_endpoint;
    Mode m_mode;

    QTcpSocket *m_tcpSocket = nullptr;
    QLocalSocket *m_localSocket = nullptr;

    bool headersParsed = false;
    QByteArray buffer;
};

#endif //AWSMOCK_QT_UI_DOCKER_LOG_CLIENT_H
