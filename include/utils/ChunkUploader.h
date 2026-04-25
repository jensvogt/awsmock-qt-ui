//
// Created by jensv on 25/04/2026.
//

#ifndef AWSMOCK_QT_UI_CHUNKUPLOADER_H
#define AWSMOCK_QT_UI_CHUNKUPLOADER_H

// ChunkedUploader.h
#pragma once
#include <QObject>
#include <QFile>
#include <QFileInfo>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QHttpMultiPart>

#define CHUNK_SIZE (8*1024*1025)

class ChunkedUploader : public QObject {
    Q_OBJECT

public:
    explicit ChunkedUploader(QObject *parent = nullptr) : QObject(parent), _manager(new QNetworkAccessManager(this)) {
    }

    void upload(const QString &filePath, const QString &url, const int chunkSize = CHUNK_SIZE) {
        _file = new QFile(filePath, this);
        if (!_file->open(QIODevice::ReadOnly)) {
            emit error("Cannot open file: " + filePath);
            return;
        }

        _url = url;
        _chunkSize = chunkSize;
        _fileName = QFileInfo(filePath).fileName();
        _fileSize = _file->size();
        _totalChunks = static_cast<long>((_fileSize + chunkSize - 1) / chunkSize);
        _currentChunk = 0;

        sendNextChunk();
    }

signals:
    void progress(int percent);

    void finished();

    void error(const QString &message);

private:
    void sendNextChunk() {
        if (_currentChunk >= _totalChunks) {
            emit finished();
            return;
        }

        const QByteArray chunkData = _file->read(_chunkSize);
        if (chunkData.isEmpty()) {
            emit finished();
            return;
        }

        auto *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

        // File chunk part
        QHttpPart filePart;
        filePart.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream");
        filePart.setHeader(QNetworkRequest::ContentDispositionHeader, QString(R"(form-data; name="file"; filename="%1")").arg(_fileName));
        filePart.setBody(chunkData);
        multiPart->append(filePart);

        // Metadata parts
        auto addField = [&](const QString &name, const QString &value) {
            QHttpPart part;
            part.setHeader(QNetworkRequest::ContentDispositionHeader, QString("form-data; name=\"%1\"").arg(name));
            part.setBody(value.toUtf8());
            multiPart->append(part);
        };

        addField("chunkIndex", QString::number(_currentChunk));
        addField("totalChunks", QString::number(_totalChunks));
        addField("fileName", _fileName);
        addField("fileSize", QString::number(_fileSize));

        QNetworkRequest request((_url));
        request.setRawHeader("X-Chunk-Index", QString::number(_currentChunk).toUtf8());
        request.setRawHeader("X-Total-Chunks", QString::number(_totalChunks).toUtf8());
        request.setRawHeader("X-File-Name", _fileName.toUtf8());

        QNetworkReply *reply = _manager->post(request, multiPart);
        multiPart->setParent(reply);

        connect(reply, &QNetworkReply::finished, this, [this, reply]() {
            reply->deleteLater();

            if (reply->error() != QNetworkReply::NoError) {
                emit error("Chunk " + QString::number(_currentChunk) + " failed: " + reply->errorString());
                return;
            }

            _currentChunk++;
            emit progress((_currentChunk * 100) / _totalChunks);
            sendNextChunk();
        });
    }

    QNetworkAccessManager *_manager;
    QFile *_file = nullptr;
    QString _url;
    QString _fileName;
    qint64 _fileSize = 0;
    qint64 _chunkSize = 0;
    long _totalChunks = 0;
    int _currentChunk = 0;
};
#endif //AWSMOCK_QT_UI_CHUNKUPLOADER_H
