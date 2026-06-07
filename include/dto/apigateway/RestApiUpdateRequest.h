#pragma once

// Awsmock includes
#include <utils/Configuration.h>

struct RestApiUpdateRequest {

    RestApiCounter restApi;

    QString ToJson() const {
        QJsonObject jRequest;
        jRequest["region"] = Configuration::instance().GetValue<QString>("awsmock.region");
        jRequest["restApi"] = restApi.toJson();
        return QJsonDocument(jRequest).toJson();
    }
};
