//
// Created by vogje01 on 6/11/26.
//

#pragma once

// Qt includes
#include <QJsonObject>
#include <QString>

struct RestApiResourceMethod {

    /**
     * @brief Is an API key required flag
     */
    bool apiKeyRequired{};

    /**
     * @brief HTTP method
     */
    QString httpMethod;

    void fromJson(const QJsonObject &jsonObject) {
        apiKeyRequired = jsonObject["apiKeyRequired"].toBool();
        httpMethod = jsonObject["httpMethod"].toString();
    }

    [[nodiscard]]
    QJsonObject toJson() const {
        QJsonObject jsonObject;
        jsonObject["apiKeyRequired"] = apiKeyRequired;
        jsonObject["httpMethod"] = httpMethod;
        return jsonObject;
    }
};
