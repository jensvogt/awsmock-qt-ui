#ifndef LIST_TOPIC_RESULT_H
#define LIST_TOPIC_RESULT_H

#include <QList>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

#include <dto/sns/SNSTopicCounter.h>

struct SNSListTopicResult {

    long total;

    QList<SNSTopicCounter> topicCounters;

    void FromJson(QJsonDocument jsonDoc) {

        QJsonArray jArray = jsonDoc["topicCounters"].toArray();

        for(const auto &element : jArray) {
            SNSTopicCounter topicCounter;
            topicCounter.FromJson(element.toObject());
            topicCounters.append(topicCounter);
        }
    }
};

#endif // LIST_TOPIC_RESULT_H
