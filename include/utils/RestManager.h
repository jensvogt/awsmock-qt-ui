#ifndef REST_MANAGER_H
#define REST_MANAGER_H

#include <QMap>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QPointer>
#include <QUrl>
#include <QTimer>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <functional>

class RestManager final : public QObject {
    Q_OBJECT

public:
    explicit RestManager(QObject *parent = nullptr);

    ~RestManager() override;

    std::function<void()> post(const QUrl &url,
                               const QByteArray &body,
                               const QMap<QString, QString> &headers,
                               std::function<void(bool success,
                                                  QByteArray response,
                                                  int status,
                                                  QString error)> callback);

    std::function<void()> get(const QUrl &url,
                              const QMap<QString, QString> &headers,
                              std::function<void(bool success,
                                                 QByteArray response,
                                                 int status,
                                                 QString error)> callback);

    std::function<void()> del(const QUrl &url,
                              const QMap<QString, QString> &headers,
                              std::function<void(bool success,
                                                 QByteArray response,
                                                 int status,
                                                 QString error)> callback);

private:
    QNetworkAccessManager m_manager;
};

#endif // REST_MANAGER_H
