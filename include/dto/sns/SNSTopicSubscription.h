#ifndef AWSMOCK_QT_UI_SNS_TOPIC_SUBSCRIPTION_H
#define AWSMOCK_QT_UI_SNS_TOPIC_SUBSCRIPTION_H

// Qt includes
#include <QList>
#include <QString>
#include <QJsonDocument>

// AwsMock
#include <utils/JsonUtils.h>

struct SNSTopicSubscription {

    /**
     * Subscription ID
     */
    QString id{};

    /**
     * Subscription ID
     */
    QString region{};

    /**
     * Subscription endpoint
     */
    QString endpoint{};

    /**
     * Subscription protocol
     */
    QString protocol{};

    /**
     * Subscription ARN
     */
    QString subscriptionArn{};

    /**
     * Subscription owner
     */
    QString owner{};

    void FromJson(const QJsonDocument &jsonDoc) {
        region = jsonDoc["region"].toString();
        id = jsonDoc["id"].toString();
        endpoint = jsonDoc["endpoint"].toString();
        protocol = jsonDoc["protocol"].toString();
        subscriptionArn = jsonDoc["subscriptionArn"].toString();
        owner = jsonDoc["owner"].toString();
    }

    void FromJson(const QJsonObject &jsonObject) {
        region = jsonObject["region"].toString();
        id = jsonObject["id"].toString();
        endpoint = jsonObject["endpoint"].toString();
        protocol = jsonObject["protocol"].toString();
        subscriptionArn = jsonObject["subscriptionArn"].toString();
        owner = jsonObject["owner"].toString();
    }

    [[nodiscard]] QString ToJson() const {
        return QJsonDocument(ToJsonObject()).toJson();
    }

    [[nodiscard]] QJsonObject ToJsonObject() const {
        QJsonObject jObject;
        jObject["region"] = region;
        jObject["id"] = id;
        jObject["endpoint"] = endpoint;
        jObject["protocol"] = protocol;
        jObject["subscriptionArn"] = subscriptionArn;
        jObject["owner"] = owner;
        return jObject;
    }
};

#endif// AWSMOCK_QT_UI_SNS_TOPIC_SUBSCRIPTION_H
