#ifndef AWSMOCK_QT_UI_SNS_TOPIC_SUBSCRIPTION_H
#define AWSMOCK_QT_UI_SNS_TOPIC_SUBSCRIPTION_H

// Qt includes
#include <QList>
#include <QString>
#include <QJsonDocument>

// AwsMock
#include <utils/JsonUtils.h>

struct SNSTopicSubscription final {

    /**
     * Subscription ID
     */
    QString id{};

    /**
     * Topic ARN
     */
    QString topicArn{};

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
        id = jsonDoc["id"].toString();
        topicArn = jsonDoc["_topicArn"].toString();
        protocol = jsonDoc["protocol"].toString();
        subscriptionArn = jsonDoc["subscriptionArn"].toString();
        owner = jsonDoc["owner"].toString();
    }

    void FromJson(const QJsonObject &jsonObject) {
        id = jsonObject["id"].toString();
        topicArn = jsonObject["_topicArn"].toString();
        protocol = jsonObject["protocol"].toString();
        subscriptionArn = jsonObject["subscriptionArn"].toString();
        owner = jsonObject["owner"].toString();
    }

    [[nodiscard]] QString ToJson() const {
        QJsonObject jObject;
        jObject["id"] = id;
        jObject["_topicArn"] = topicArn;
        jObject["protocol"] = protocol;
        jObject["subscriptionArn"] = subscriptionArn;
        jObject["owner"] = owner;
        const QJsonDocument jDoc(jObject);
        return jDoc.toJson();
    }
};

#endif// AWSMOCK_QT_UI_SNS_TOPIC_SUBSCRIPTION_H
