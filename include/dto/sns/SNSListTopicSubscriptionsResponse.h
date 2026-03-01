//
// Created by vogje01 on 12/20/25.
//

#ifndef AWSMOCK_QT_UI_SNS_LIST_TOPIC_SUBSCRIPTIONS_RESPONSE_H
#define AWSMOCK_QT_UI_SNS_LIST_TOPIC_SUBSCRIPTIONS_RESPONSE_H

// Qt includes
#include <QList>
#include <QJsonArray>

// AwsMock includes
#include <dto/sns/SNSTopicSubscription.h>

struct ListTopicSubscriptionsResponse {

    int total{};
    QList<SNSTopicSubscription> topicSubscriptions{};

    void FromJson(const QJsonDocument &jsonDoc) {

        total = jsonDoc.object()["total"].toInt();

        for (QJsonArray jArray = jsonDoc["subscriptionCounters"].toArray(); const auto &element: jArray) {
            SNSTopicSubscription topicSubscription;
            topicSubscription.FromJson(element.toObject());
            topicSubscriptions.append(topicSubscription);
        }
    }
};
#endif //AWSMOCK_QT_UI_SNS_LIST_TOPIC_SUBSCRIPTIONS_RESPONSE_H
