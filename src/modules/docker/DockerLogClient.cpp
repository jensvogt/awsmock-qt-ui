//
// Created by vogje01 on 11/23/25.
//


#include <modules/docker/DockerLogClient.h>
#include <QDebug>

#include "dto/dashboard/DashboardCounterResult.h"

DockerLogClient::DockerLogClient(const QString &containerId, const Mode mode, const QString &endpoint,
                                 QObject *parent) : QObject(parent), m_containerId(containerId), m_endpoint(endpoint),
                                                    m_mode(mode) {
    if (mode == Mode::Tcp) {
        m_tcpSocket = new QTcpSocket(this);
        connect(m_tcpSocket, &QTcpSocket::connected, this, &DockerLogClient::onConnected);
        connect(m_tcpSocket, &QTcpSocket::readyRead, this, &DockerLogClient::onReadyRead);
        connect(m_tcpSocket, &QTcpSocket::errorOccurred, this, &DockerLogClient::onErrorOccurred);
    } else {
        m_localSocket = new QLocalSocket(this);
        connect(m_localSocket, &QLocalSocket::connected, this, &DockerLogClient::onConnected);
        connect(m_localSocket, &QLocalSocket::readyRead, this, &DockerLogClient::onReadyRead);
        connect(m_localSocket, &QLocalSocket::errorOccurred, this, &DockerLogClient::onErrorOccurred);
    }
}

void DockerLogClient::ConnectToDocker() const {
    if (m_mode == Mode::Tcp) {
        QStringList parts = m_endpoint.split(':');
        const QString host = parts[0];
        const quint16 port = parts[1].toUInt();
        m_tcpSocket->connectToHost(host, port);
    } else {
        m_localSocket->connectToServer(m_endpoint);
    }
}

void DockerLogClient::DisconnectFromDocker() const {
    if (m_tcpSocket) m_tcpSocket->close();
    if (m_localSocket) m_localSocket->close();
}

void DockerLogClient::onConnected() {
    emit Connected();
    SendRequest();
}

void DockerLogClient::SendRequest() const {
    const QByteArray req = "GET /containers/" + m_containerId.toUtf8() + "/logs?stdout=1&stderr=1&follow=1 HTTP/1.1\r\n"
                           "Host: docker\r\n"
                           "Connection: Upgrade\r\n"
                           "Upgrade: tcp\r\n"
                           "\r\n";

    if (m_tcpSocket) {
        m_tcpSocket->write(req);
    } else {
        m_localSocket->write(req);
    }
}

void DockerLogClient::onReadyRead() {
    QByteArray data;

    if (m_tcpSocket)
        data = m_tcpSocket->readAll();
    else
        data = m_localSocket->readAll();

    buffer.append(data);

    // Step 1: parse HTTP headers once
    if (!headersParsed) {
        const int headerEnd = buffer.indexOf("\r\n\r\n");
        if (headerEnd == -1) return;

        buffer.remove(0, headerEnd + 4);
        headersParsed = true;
    }

    // Step 2: Docker multiplexed stream
    // Each frame: [stream][0][0][0][size1][size2][size3][size4][payload]
    while (buffer.size() >= 8) {
        uchar stream = buffer[0];
        quint32 size =
                ((quint8) buffer[4] << 24) |
                ((quint8) buffer[5] << 16) |
                ((quint8) buffer[6] << 8) |
                ((quint8) buffer[7]);

        //if (buffer.size() < 8 + size)
        //    return; // Not enough data yet

        QByteArray msg = buffer.mid(8, size);

        // Remove frame from buffer
        buffer.remove(0, 8 + size);

        QString logMessage = QString::fromUtf8(msg);
        static QRegularExpression ansi("\\x1B\\[[0-9;]*[mK]");
        logMessage.remove(ansi);

        emit LogReceived(SanitizeString(logMessage));
    }
}

void DockerLogClient::onErrorOccurred() {
    QString msg;
    if (m_tcpSocket) msg = m_tcpSocket->errorString();
    if (m_localSocket) msg = m_localSocket->errorString();
    emit ErrorOccurred(msg);
}

QStringList DockerLogClient::SanitizeString(const QString &input) {
    QStringList lines = input.split('\n', Qt::SkipEmptyParts);
    QStringList cleaned;

    // Regex for lines that contain only a hexadecimal number
    const QRegularExpression hexLine("^[0-9a-fA-F]+$");

    for (QString line: lines) {
        // Remove unprintable characters
        line.remove(QRegularExpression("[^\\x20-\\x7E]"));

        // Skip lines that are just a hex number
        if (hexLine.match(line.trimmed()).hasMatch()) {
            continue;
        }
        line.remove(QRegularExpression("[\\r\\n]"));
        if (!line.trimmed().isEmpty()) {
            cleaned.append(line);
        }
    }

    return cleaned;
}
