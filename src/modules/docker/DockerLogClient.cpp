//
// Created by vogje01 on 11/23/25.
//


#include <modules/docker/DockerLogClient.h>

#include <utility>

DockerLogClient::DockerLogClient(QString containerId, const Mode mode, QString endpoint, const long limit, QObject *parent)
    : QObject(parent), m_containerId(std::move(containerId)), m_endpoint(std::move(endpoint)), m_mode(mode), _limit(limit) {
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
        const QString &host = parts[0];
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
    SendRequest(_limit);
}

void DockerLogClient::SendRequest(const long limit) const {
    const QByteArray req = "GET /containers/" + m_containerId.toUtf8() + "/logs?stdout=1&stderr=1&follow=1&tail=" + std::to_string(limit) + " HTTP/1.1\r\n"
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
        const int headerEnd = static_cast<int>(buffer.indexOf("\r\n\r\n"));
        if (headerEnd == -1) return;

        // Check if chunked transfer encoding
        QString headers = QString::fromLatin1(buffer.left(headerEnd));
        _chunked = headers.contains("Transfer-Encoding: chunked", Qt::CaseInsensitive);

        buffer.remove(0, headerEnd + 4);
        headersParsed = true;
    }

    // Step 2: strip chunked encoding if needed
    if (_chunked) {
        buffer = StripChunkedEncoding(buffer);
    }

    // Step 3: Docker multiplexed stream
    // Each frame: [stream(1)][0][0][0][size(4 big-endian)][payload]
    while (buffer.size() >= 8) {

        // Validate stream type (0=stdin, 1=stdout, 2=stderr)
        if (const uchar stream = static_cast<uchar>(buffer[0]); stream > 2) {
            // Out of sync — scan forward for a valid header
            buffer.remove(0, 1);
            continue;
        }

        const quint32 size =
                static_cast<quint8>(buffer[4]) << 24 |
                static_cast<quint8>(buffer[5]) << 16 |
                static_cast<quint8>(buffer[6]) << 8 |
                static_cast<quint8>(buffer[7]);

        // Wait until full frame is available
        if (buffer.size() < static_cast<int>(8 + size)) break;

        QByteArray msg = buffer.mid(8, size);
        buffer.remove(0, 8 + size);

        // Skip empty frames
        if (msg.isEmpty()) continue;

        QString logMessage = QString::fromUtf8(msg);

        // Remove ANSI escape codes
        static const QRegularExpression ansi("\x1B(?:[@-Z\\-_]|\\[[0-?]*[ -/]*[@-~])");
        logMessage.remove(ansi);

        emit LogReceived(SanitizeString(logMessage));
    }
}

QByteArray DockerLogClient::StripChunkedEncoding(const QByteArray &input) {
    QByteArray result;
    int i = 0;

    while (i < input.size()) {
        // Find end of chunk size line
        const int lineEnd = input.indexOf("\r\n", i);
        if (lineEnd == -1) break;

        // Parse hex chunk size
        bool ok = false;
        const quint64 chunkSize = input.mid(i, lineEnd - i).trimmed().toULongLong(&ok, 16);
        if (!ok) break;

        // Terminal chunk
        if (chunkSize == 0) break;

        const int dataStart = lineEnd + 2;
        if (dataStart + static_cast<int>(chunkSize) > input.size()) break;

        result.append(input.mid(dataStart, chunkSize));
        i = dataStart + chunkSize + 2; // skip trailing \r\n
    }

    return result.isEmpty() ? input : result;
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
    const QRegularExpression colorCoding("[^\\x20-\\x7E]");
    const QRegularExpression lineEndings("[\\r\\n]");

    for (QString line: lines) {
        // Remove unprintable characters
        line.remove(colorCoding);

        // Skip lines that are just a hex number
        if (hexLine.match(line.trimmed()).hasMatch()) {
            continue;
        }
        line.remove(lineEndings);
        if (!line.trimmed().isEmpty()) {
            cleaned.append(line.toUtf8());
        }
    }

    return cleaned;
}
