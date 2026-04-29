#ifndef AWSMOCK_QT_UI_SNS_SUBSCRIPTION_RESPONSE_H
#define AWSMOCK_QT_UI_SNS_SUBSCRIPTION_RESPONSE_H

// Qt includes
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QXmlStreamReader>

// AwsMock includes
#include <dto/sns/SNSMessageAttribute.h>

struct SNSSubscriptionResponse {

    SNSTopicSubscription snsTopicSubscription;

    void FromJson(const QJsonDocument &jsonDoc) {
        snsTopicSubscription.FromJson(jsonDoc["subscriptionCounter"].toObject());
    }

    [[nodiscard]] QByteArray ToJson() const {
        return QJsonDocument(snsTopicSubscription.ToJsonObject()).toJson();
    }
};

#endif // AWSMOCK_QT_UI_SNS_SUBSCRIPTION_RESPONSE_H
