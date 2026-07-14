#pragma once

// Qt includes
#include <QDateTime>
#include <QJsonObject>

struct UsagePlan {

    QString id;

    QString name;

    QString description;

    double throttleRateLimit{};

    int throttleBurstLimit{};

    int quotaLimit{};

    QString quotaPeriod;

    QDateTime created;

    QDateTime modified;

    void fromJson(const QJsonObject &jsonObject) {
        id = jsonObject["id"].toString();
        name = jsonObject["name"].toString();
        description = jsonObject["description"].toString();
        if (jsonObject.contains("throttle") && jsonObject["throttle"].isObject()) {
            const QJsonObject throttle = jsonObject["throttle"].toObject();
            throttleRateLimit = throttle["rateLimit"].toDouble();
            throttleBurstLimit = throttle["burstLimit"].toInt();
        }
        if (jsonObject.contains("quota") && jsonObject["quota"].isObject()) {
            const QJsonObject quota = jsonObject["quota"].toObject();
            quotaLimit = quota["limit"].toInt();
            quotaPeriod = quota["period"].toString();
        }
        created = QDateTime::fromString(jsonObject["created"].toString(), Qt::ISODateWithMs);
        modified = QDateTime::fromString(jsonObject["modified"].toString(), Qt::ISODateWithMs);
    }

    [[nodiscard]]
    QJsonObject toJson() const {
        QJsonObject jsonObject;
        jsonObject["id"] = id;
        jsonObject["name"] = name;
        jsonObject["description"] = description;
        QJsonObject throttle;
        throttle["rateLimit"] = throttleRateLimit;
        throttle["burstLimit"] = throttleBurstLimit;
        jsonObject["throttle"] = throttle;
        QJsonObject quota;
        quota["limit"] = quotaLimit;
        quota["period"] = quotaPeriod;
        jsonObject["quota"] = quota;
        jsonObject["created"] = created.toString(Qt::ISODateWithMs);
        jsonObject["modified"] = modified.toString(Qt::ISODateWithMs);
        return jsonObject;
    }
};
