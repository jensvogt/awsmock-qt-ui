//
// Created by vogje01 on 2/15/26.
//

#ifndef AWSMOCK_QT_UI_COGNITO_USERPOOL_LIST_RESPONSE_H
#define AWSMOCK_QT_UI_COGNITO_USERPOOL_LIST_RESPONSE_H

// Qt includes
#include <QList>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

// Awsmock includes
#include <dto/cognito/CognitoUserpool.h>

struct CognitoUserpoolListResponse {
    long total{};

    QList<CognitoUserpool> userpools;

    void FromJson(const QJsonDocument &jsonDoc) {
        total = jsonDoc.object()["total"].toInt();
        for (QJsonArray jArray = jsonDoc["bucketCounters"].toArray(); const auto &element: jArray) {
            CognitoUserpool userpool;
            userpool.FromJson(element.toObject());
            userpools.append(userpool);
        }
    }
};

#endif //AWSMOCK_QT_UI_COGNITO_USERPOOL_LIST_RESPONSE_H
