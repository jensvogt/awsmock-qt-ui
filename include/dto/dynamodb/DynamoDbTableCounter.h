#ifndef AWSMOCK_QT_UI_DYNAMODB_TABLE_COUNTER_H
#define AWSMOCK_QT_UI_DYNAMODB_TABLE_COUNTER_H

#include <QDateTime>
#include <QJsonObject>

struct DynamoDbTableCounter {
    QString region;

    QString tableName;

    QString tableArn;

    long itemCount;

    long size;

    QDateTime created;

    QDateTime modified;

    void FromJson(const QJsonObject &jsonObject) {
        region = jsonObject["region"].toString();
        tableName = jsonObject["tableName"].toString();
        tableArn = jsonObject["tableArn"].toString();
        itemCount = jsonObject["items"].toInteger();
        size = jsonObject["size"].toInteger();
        created = QDateTime::fromString(jsonObject["created"].toString(), Qt::ISODate);
        modified = QDateTime::fromString(jsonObject["modified"].toString(), Qt::ISODate);
    }
};


#endif // AWSMOCK_QT_UI_DYNAMODB_TABLE_COUNTER_H
