#ifndef AWSMOCK_QT_UI_SNS_TOPIC_TAG_H
#define AWSMOCK_QT_UI_SNS_TOPIC_TAG_H

// Qt includes
#include <QList>
#include <QString>
#include <QJsonDocument>

// AwsMock
#include <utils/JsonUtils.h>

struct SNSTopicTag final {

    /**
     * Topic tag name
     */
    QString name{};

    /**
     * Message tag value
     */
    QString value{};

    void FromJson(const QJsonDocument &jsonDoc) {
        name = jsonDoc["tagKey"].toString();
        value = jsonDoc["tagValue"].toString();
    }

    void FromJson(const QJsonObject &jsonObject) {
        name = jsonObject["tagKey"].toString();
        value = jsonObject["tagValue"].toString();
    }

    [[nodiscard]] QString ToJson() const {
        QJsonObject jObject;
        jObject["tagKey"] = name;
        jObject["tagValue"] = value;
        const QJsonDocument jDoc(jObject);
        return jDoc.toJson();
    }
};

#endif// AWSMOCK_QT_UI_SNS_TOPIC_TAG_H
