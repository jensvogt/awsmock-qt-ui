#ifndef AWSMOCK_QT_UI_SECRETSMANAGER_SECRET_VERSION_COUNTER_H
#define AWSMOCK_QT_UI_SECRETSMANAGER_SECRET_VERSION_COUNTER_H


// Qt includes
#include <QString>
#include <QDateTime>
#include <QJsonArray>
#include <QJsonObject>

/**
 * @brief Secrets version counter
 *
 * @author jens.vogt\@opitz-consulting.com
 */
struct SecretVersionCounter {

    /**
     * Secret version ID
     */
    QString versionId;

    /**
     * States
     */
    QVector<QString> states;

    /**
     * Created timestamp
     */
    QDateTime created;

    /**
     * Modified timestamp
     */
    QDateTime modified;

    void FromJson(const QJsonObject &jsonObject) {
        versionId = jsonObject["versionId"].toString();
        created = QDateTime::fromString(jsonObject["created"].toString(), Qt::ISODate);
        modified = QDateTime::fromString(jsonObject["modified"].toString(), Qt::ISODate);
        if (jsonObject["states"].isArray()) {
            for (int i = 0; i < jsonObject["states"].toArray().size(); i++) {
                states.append(jsonObject["states"].toArray()[i].toString());
            }
        }
    }

    QJsonObject ToJsonObject() const {
        QJsonObject jsonObject;
        jsonObject["versionId"] = versionId;
        jsonObject["created"] = created.toString(Qt::ISODate);
        jsonObject["modified"] = modified.toString(Qt::ISODate);
        if (!states.empty()) {
            QJsonArray jsonArray;
            for (const QString &state: states) {
                jsonArray.append(jsonObject[state]);
            }
            jsonObject["states"] = jsonArray;
        }
        return jsonObject;
    }
};

#endif// AWSMOCK_QT_UI_SECRETSMANAGER_SECRET_VERSION_COUNTER_H
