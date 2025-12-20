//
// Created by vogje01 on 12/20/25.
//

#ifndef AWSMOCK_QT_UI_SNS_LIST_TOPIC_ATTRIBUTE_COUNTERS_RESPONSE_H
#define AWSMOCK_QT_UI_SNS_LIST_TOPIC_ATTRIBUTE_COUNTERS_RESPONSE_H

// Qt includes
#include <QList>

// AwsMock includes
#include <dto/sns/SNSTopicAttribute.h>

struct ListTopicAttributesCountersResponse {

    int total{};
 QList<SNSTopicAttribute> topicAttributes{};

    void FromJson(const QJsonDocument &jsonDoc) {

        total = jsonDoc.object()["total"].toInt();

        for (QJsonArray jArray = jsonDoc["attributeCounters"].toArray(); const auto &element: jArray) {
            SNSTopicAttribute topicAttribute;
            topicAttribute.FromJson(element.toObject());
            topicAttributes.append(topicAttribute);
        }
    }
};
#endif //AWSMOCK_QT_UI_SNS_LIST_TOPIC_ATTRIBUTE_COUNTERS_RESPONSE_H
