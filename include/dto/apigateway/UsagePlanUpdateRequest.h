#pragma once

// Qt includes
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>

struct UsagePlanUpdateRequest {

    QString planId;

    QString name;

    QString description;

    double throttleRateLimit{};

    int throttleBurstLimit{};

    int quotaLimit{};

    QString quotaPeriod;

    [[nodiscard]]
    QString ToJson() const {
        QJsonObject jRequest;
        jRequest["usagePlanId"] = planId;
        jRequest["name"] = name;
        jRequest["description"] = description;
        QJsonObject throttle;
        throttle["rateLimit"] = throttleRateLimit;
        throttle["burstLimit"] = throttleBurstLimit;
        jRequest["throttle"] = throttle;
        QJsonObject quota;
        quota["limit"] = quotaLimit;
        quota["period"] = quotaPeriod;
        jRequest["quota"] = quota;
        return QJsonDocument(jRequest).toJson();
    }
};
