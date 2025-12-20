#ifndef AWSMOCK_QT_UI_SNS_TOPIC_ATTRIBUTE_H
#define AWSMOCK_QT_UI_SNS_TOPIC_ATTRIBUTE_H

// Qt includes
#include <QList>
#include <QString>
#include <QJsonDocument>

// AwsMock
#include <utils/JsonUtils.h>

struct SNSTopicAttribute final {

    /**
     * Topic attribute value
     */
    QString key{};

    /**
     * Message attribute number value
     */
    QString value{};

    void FromJson(const QJsonDocument &jsonDoc) {
        key = jsonDoc["attributeKey"].toString();
        value = jsonDoc["attributeValue"].toString();
    }

    void FromJson(const QJsonObject &jsonObject) {
        key = jsonObject["attributeKey"].toString();
        value = jsonObject["attributeValue"].toString();
    }

    [[nodiscard]] QString ToJson() const {
        QJsonObject jObject;
        jObject["attributeKey"] = key;
        jObject["attributeValue"] = value;
        const QJsonDocument jDoc(jObject);
        return jDoc.toJson();
    }
};

#endif// AWSMOCK_QT_UI_SNS_TOPIC_ATTRIBUTE_H
