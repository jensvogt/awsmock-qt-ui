//
// Created by vogje01 on 11/23/25.
//

#include <modules/docker/DockerLogClient.h>

DockerLogClient::DockerLogClient(QString containerId, QString endpoint, const long limit, QObject *parent) : QObject(parent), m_containerId(std::move(containerId)), m_endpoint(std::move(endpoint)), _limit(limit) {
    m_webSocket = new QWebSocket(QString(), QWebSocketProtocol::VersionLatest, this);
    connect(m_webSocket, &QWebSocket::connected, this, &DockerLogClient::onConnected);
    connect(m_webSocket, &QWebSocket::textMessageReceived, this, &DockerLogClient::onTextMessageReceived);
    connect(m_webSocket, &QWebSocket::disconnected, this, &DockerLogClient::onDisconnected);
    connect(m_webSocket, &QWebSocket::errorOccurred, this, &DockerLogClient::onErrorOccurred);
}

void DockerLogClient::ConnectToDocker() const {
    QUrl url(m_endpoint);
    QUrlQuery query;
    query.addQueryItem("containerId", m_containerId);
    url.setQuery(query);
    m_webSocket->open(url);
}

void DockerLogClient::DisconnectFromDocker() const {
    m_webSocket->close();
}

void DockerLogClient::onConnected() {
    emit Connected();
}

void DockerLogClient::onTextMessageReceived(const QString &message) {
    if (const QStringList lines = SanitizeString(message); !lines.isEmpty()) {
        emit LogReceived(lines);
    }
}

void DockerLogClient::onDisconnected() {
    //m_pollTimer->stop();
    if (m_hasError) {
        emit Disconnected();
    }
    m_hasError = false;
}

void DockerLogClient::onErrorOccurred(QAbstractSocket::SocketError) {
    m_hasError = true;
    logError << m_webSocket->errorString();
    emit ErrorOccurred(m_webSocket->errorString());
}

QStringList DockerLogClient::SanitizeString(const QString &input) {
    QStringList lines = input.split('\n', Qt::SkipEmptyParts);
    QStringList cleaned;

    static const QRegularExpression ansi("\x1B(?:[@-Z\\-_]|\\[[0-?]*[ -/]*[@-~])");
    static const QRegularExpression hexLine("^[0-9a-fA-F]+$");
    static const QRegularExpression unprintable("[^\\x20-\\x7E]");
    static const QRegularExpression lineEndings("[\\r\\n]");

    for (QString line: lines) {
        line.remove(ansi);
        line.remove(unprintable);
        if (hexLine.match(line.trimmed()).hasMatch()) continue;
        line.remove(lineEndings);
        if (!line.trimmed().isEmpty()) {
            cleaned.append(line);
        }
    }
    return cleaned;
}
