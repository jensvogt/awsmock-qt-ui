#ifndef AWSMOCK_QT_UI_SSM_PARAMETER_PUT_RESULT_H
#define AWSMOCK_QT_UI_SSM_PARAMETER_PUT_RESULT_H

#include <QJsonObject>
#include <QJsonDocument>

struct SSMParameterPutResponse {

    QString tier;

    long version;

    void FromJson(const QJsonDocument &jsonDoc) {
        tier = jsonDoc["Tier"].toString();
        version = jsonDoc["Version"].toInt();
    }
};

#endif // AWSMOCK_QT_UI_SSM_PARAMETER_PUT_RESULT_H
