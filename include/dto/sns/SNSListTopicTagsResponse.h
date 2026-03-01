//
// Created by vogje01 on 12/20/25.
//

#ifndef AWSMOCK_QT_UI_SNS_LIST_TOPIC_TAGS_RESPONSE_H
#define AWSMOCK_QT_UI_SNS_LIST_TOPIC_TAGS_RESPONSE_H

// Qt includes
#include <QList>

// AwsMock includes
#include <dto/sns/SNSTopicTag.h>

struct ListTopicTagsResponse {

    int total{};
    QList<SNSTopicTag> topicTags{};

    void FromJson(const QJsonDocument &jsonDoc) {

        total = jsonDoc.object()["total"].toInt();

        for (QJsonArray jArray = jsonDoc["attributeCounters"].toArray(); const auto &element: jArray) {
            SNSTopicTag topicTag;
            topicTag.FromJson(element.toObject());
            topicTags.append(topicTag);
        }
    }
};
#endif //AWSMOCK_QT_UI_SNS_LIST_TOPIC_TAGS_RESPONSE_H
