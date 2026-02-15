#ifndef AWSMOCK_QT_UI_COGNITO_USERPOOL_COUNTER_H
#define AWSMOCK_QT_UI_COGNITO_USERPOOL_COUNTER_H

// Qt includes
#include <QDateTime>
#include <QJsonObject>

struct CognitoUserpool {
    QString region;

    QString id;

    QString name;

    QString userpoolId;

    QString arn;

    QString domain;

    long userCount;

    QDateTime created;

    QDateTime modified;

    void FromJson(const QJsonObject &jsonObject) {
        region = jsonObject["region"].toString();
        id = jsonObject["id"].toString();
        name = jsonObject["name"].toString();
        userpoolId = jsonObject["userpoolId"].toString();
        arn = jsonObject["arn"].toString();
        domain = jsonObject["domain"].toString();
        userCount = jsonObject["userCount"].toInt();
        created = QDateTime::fromString(jsonObject["created"].toString(), Qt::ISODate);
        modified = QDateTime::fromString(jsonObject["modified"].toString(), Qt::ISODate);
    }
};


#endif // AWSMOCK_QT_UI_COGNITO_USERPOOL_COUNTER_H
