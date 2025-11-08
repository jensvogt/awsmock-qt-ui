#include <QNetworkRequest>
#include <utils/RestManager.h>

RestManager::RestManager(QObject *parent): QObject(parent), m_manager(this) {
}

RestManager::~RestManager() = default;

std::function<void()> RestManager::post(const QUrl &url, const QByteArray &body, const QMap<QString, QString> &headers, std::function<void(bool, QByteArray, int, QString)> callback) {

    QNetworkRequest req(url);

    // default content-type; caller may override in headers
    if (!req.rawHeader("Content-Type").size())
        req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // apply headers
    for (auto it = headers.constBegin(); it != headers.constEnd(); ++it) {
        req.setRawHeader(it.key().toUtf8(), it.value().toUtf8());
    }

    QNetworkReply *reply = m_manager.post(req, body);
    // Use QPointer so the cancel functor can safely check if reply still exists.
    QPointer replyPtr(reply);

    // ensure callback gets called once when finished (or on error)
    connect(reply, &QNetworkReply::finished, this, [reply, callback]() mutable {
        if (!reply) {
            if (callback)
                callback(false, {}, 0, QStringLiteral("Reply pointer null"));
            return;
        }

        // read body
        const QByteArray resp = reply->readAll();
        // extract HTTP status code (may be invalid if not set)
        const QVariant statusAttr = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
        const int status = statusAttr.isValid() ? statusAttr.toInt() : 0;

        const bool success = (reply->error() == QNetworkReply::NoError);
        const QString err = reply->error() == QNetworkReply::NoError ? QString() : reply->errorString();

        // call user callback
        if (callback) {
            callback(success, resp, status, err);
        }

        // delete reply later
        reply->deleteLater();
    });

    // Also handle network errors that may be emitted before finished (optional).
    connect(reply,
            &QNetworkReply::errorOccurred,
            this,
            [reply](QNetworkReply::NetworkError) {
                // nothing special here since finished() will run and we call callback there.
                // But you could emit progress or log here if you want.
                Q_UNUSED(reply);
            });

    // return a cancel functor
    return [replyPtr]() mutable {
        if (replyPtr) {
            // abort() will cause the reply to finish and trigger the finished() handler.
            replyPtr->abort();
        }
        // if replyPtr is null (already finished/deleted), it's a no-op
    };
}
