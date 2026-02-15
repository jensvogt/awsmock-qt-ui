#ifndef AWSMOCK_QT_UI_COGNITO_USERPOOL_COUNTER_H
#define AWSMOCK_QT_UI_COGNITO_USERPOOL_COUNTER_H

// Qt includes
#include <QDateTime>
#include <QJsonObject>

struct CognitoUserpool {
    QString region;

    QString name;

    QDateTime created;

    QDateTime modified;

    void FromJson(const QJsonObject &jsonObject) {
        region = jsonObject["region"].toString();
        name = jsonObject["name"].toString();
        created = QDateTime::fromString(jsonObject["created"].toString(), Qt::ISODate);
        modified = QDateTime::fromString(jsonObject["modified"].toString(), Qt::ISODate);
    }
};


#endif // AWSMOCK_QT_UI_COGNITO_USERPOOL_COUNTER_H
