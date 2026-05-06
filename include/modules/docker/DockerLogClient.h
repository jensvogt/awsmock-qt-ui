//
// Created by vogje01 on 11/23/25.
//

#ifndef AWSMOCK_QT_UI_DOCKER_LOG_CLIENT_H
#define AWSMOCK_QT_UI_DOCKER_LOG_CLIENT_H

// Qt includes
#include <QDebug>
#include <QLocalSocket>
#include <QObject>
#include <QRegularExpression>
#include <QTcpSocket>

class DockerLogClient final : public QObject {
    Q_OBJECT

public:
    enum class Mode {
        UnixSocket,
        Tcp
    };

    DockerLogClient(QString containerId, Mode mode, QString endpoint, long limit, QObject *parent);

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
    static QByteArray StripChunkedEncoding(const QByteArray &input);

    void SendRequest(long limit) const;

    QString m_containerId;
    QString m_endpoint;
    Mode m_mode;

    QTcpSocket *m_tcpSocket = nullptr;
    QLocalSocket *m_localSocket = nullptr;

    bool headersParsed = false;
    QByteArray buffer;

    /**
     * @brief Maximal number of log lines
     */
    long _limit = 1000;

    /**
     * @brief Chunked flag
     */
    bool _chunked = false;
};

#endif //AWSMOCK_QT_UI_DOCKER_LOG_CLIENT_H
