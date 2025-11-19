#include <utils/RestManager.h>
#include <QNetworkReply>
#include <QTimer>

RestManager::RestManager(QObject *parent)
    : QObject(parent) {
}

RestManager::~RestManager() = default;


// ---- internal helper ----

static std::function<void()> makeRequest(
    QNetworkAccessManager &mgr,
    QNetworkAccessManager::Operation op,
    const QUrl &url,
    const QByteArray &body,
    const QMap<QString, QString> &headers,
    std::function<void(bool, QByteArray, int, QString)> callback) {
    QNetworkRequest req(url);
    for (auto it = headers.begin(); it != headers.end(); ++it)
        req.setRawHeader(it.key().toUtf8(), it.value().toUtf8());

    QNetworkReply *reply = nullptr;

    switch (op) {
        case QNetworkAccessManager::PostOperation:
            reply = mgr.post(req, body);
            break;
        case QNetworkAccessManager::GetOperation:
            reply = mgr.get(req);
            break;
        case QNetworkAccessManager::DeleteOperation:
            reply = mgr.deleteResource(req);
            break;
        default:
            Q_ASSERT(false);
    }

    QObject::connect(reply, &QNetworkReply::finished, reply, [reply, callback]() {
        const int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        const QByteArray data = reply->readAll();
        const QString error = reply->errorString();
        const bool success = (reply->error() == QNetworkReply::NoError);

        reply->deleteLater();
        callback(success, data, status, error);
    });

    return [reply]() {
        if (reply)
            reply->abort();
    };
}


// ----------- public wrappers -----------

std::function<void()> RestManager::post(const QUrl &url,
                                        const QByteArray &body,
                                        const QMap<QString, QString> &headers,
                                        std::function<void(bool, QByteArray, int, QString)> callback) {
    return makeRequest(m_manager, QNetworkAccessManager::PostOperation, url, body, headers, std::move(callback));
}

std::function<void()> RestManager::get(const QUrl &url,
                                       const QMap<QString, QString> &headers,
                                       std::function<void(bool, QByteArray, int, QString)> callback) {
    return makeRequest(m_manager, QNetworkAccessManager::GetOperation, url, {}, headers, std::move(callback));
}

std::function<void()> RestManager::del(const QUrl &url,
                                       const QMap<QString, QString> &headers,
                                       std::function<void(bool, QByteArray, int, QString)> callback) {
    return makeRequest(m_manager, QNetworkAccessManager::DeleteOperation, url, {}, headers, std::move(callback));
}
