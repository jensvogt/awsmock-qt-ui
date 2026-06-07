#pragma once

// Awsmock includes
#include <utils/Configuration.h>
#include <dto/apigateway/RestApiCounter.h>

struct RestApiCreateRequest {

    QString name;

    QString description;

    bool enabled;

    QString ToJson() const {
        QJsonObject jRequest;
        jRequest["region"] = Configuration::instance().GetValue<QString>("awsmock.region");
        jRequest["name"] = name;
        jRequest["description"] = description;
        jRequest["enabled"] = enabled;
        return QJsonDocument(jRequest).toJson();
    }
};
