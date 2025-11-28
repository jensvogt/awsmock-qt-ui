#ifndef AWSMOCK_QT_UI_LAMBDA_RESULT_COUNTER_H
#define AWSMOCK_QT_UI_LAMBDA_RESULT_COUNTER_H

#include <QDateTime>
#include <QJsonObject>

struct LambdaResultCounter {

    QString oid;

    QString lambdaName;

    QString lambdaArn;

    QString runtime;

    QString requestBody;

    QString responseBody;

    QString logMessages;

    long duration;

    QString instanceId;

    QString containerId;

    QString lambdaStatus;

    int httpStatusCode;

    QDateTime timestamp;

    void FromJson(const QJsonObject &jsonObject) {
        oid = jsonObject["oid"].toString();
        lambdaName = jsonObject["lambdaName"].toString();
        lambdaArn = jsonObject["lambdaArn"].toString();
        runtime = jsonObject["runtime"].toString();
        requestBody = QByteArray::fromBase64(jsonObject["requestBody"].toString().toUtf8());
        responseBody = QByteArray::fromBase64(jsonObject["responseBody"].toString().toUtf8());
        logMessages = QByteArray::fromBase64(jsonObject["logMessages"].toString().toUtf8());
        duration = jsonObject["duration"].toInt();
        instanceId = jsonObject["instanceId"].toString();
        containerId = jsonObject["containerId"].toString();
        lambdaStatus = jsonObject["lambdaStatus"].toString();
        httpStatusCode = jsonObject["httpStatusCode"].toInt();
        timestamp = QDateTime::fromString(jsonObject["timestamp"].toString(), Qt::ISODate);
    }
};


#endif // AWSMOCK_QT_UI_LAMBDA_RESULT_COUNTER_H
