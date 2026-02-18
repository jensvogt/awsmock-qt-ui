//
// Created by jensv on 18/02/2026.
//

#ifndef AWSMOCK_QT_UI_LIST_MODULE_NAMES_RESPONSE_H
#define AWSMOCK_QT_UI_LIST_MODULE_NAMES_RESPONSE_H

// Qt includes
#include <QVector>
#include <QJsonObject>
#include <QJsonArray>

struct ListModuleNamesResponse {
    /**
     * Module names list
     */
    QVector<QString> moduleNames;

    void FromJson(const QJsonObject &jsonObject) {

        if (jsonObject.contains("modules")) {
            for (const auto &moduleName: jsonObject["modules"].toArray()) {
                moduleNames.append(moduleName.toString());
            }
        }
    }
};

#endif // AWSMOCK_QT_UI_LIST_MODULE_NAMES_RESPONSE_H
