#ifndef AWSMOCK_QT_UI_LAMBDA_INSTANCE_COUNTER_H
#define AWSMOCK_QT_UI_LAMBDA_INSTANCE_COUNTER_H

#include <QDateTime>
#include <QJsonObject>

struct LambdaInstanceCounter {
    QString instanceId;

    QString containerId;

    QString status;

    QString hostname;

    int duration;

    int port;

    QDateTime lastInvocation;

    void FromJson(const QJsonObject &jsonObject) {
        instanceId = jsonObject["instanceId"].toString();
        containerId = jsonObject["containerId"].toString();
        status = jsonObject["status"].toString();
        hostname = jsonObject["hostname"].toString();
        duration = jsonObject["duration"].toInt();
        port = jsonObject["port"].toInt();
        lastInvocation = QDateTime::fromString(jsonObject["lastInvocation"].toString(), Qt::ISODate);
    }
};


#endif // AWSMOCK_QT_UI_LAMBDA_INSTANCE_COUNTER_H
