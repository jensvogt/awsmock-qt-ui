#ifndef REST_MANAGER_H
#define REST_MANAGER_H

// NetworkManager.h
#pragma once
#include <QMap>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QPointer>
#include <QUrl>
#include <functional>

class RestManager : public QObject
{
    Q_OBJECT
public:
    explicit RestManager(QObject *parent = nullptr);
    ~RestManager();

    // Performs a POST. Returns a cancel functor: call it to abort the request.
    // callback: void(success, responseBody, httpStatus, errorString)
    std::function<void()> post(const QUrl &url,
                               const QByteArray &body,
                               const QMap<QString, QString> &headers,
                               std::function<void(bool, QByteArray, int, QString)> callback);

private:
    QNetworkAccessManager m_manager;
};

#endif // REST_MANAGER_H
