#ifndef AWSMOCK_QT_UI_SECRETSMANAGER_SECRETSMANAGER_SECRET_H
#define AWSMOCK_QT_UI_SECRETSMANAGER_SECRETSMANAGER_SECRET_H


// Qt includes
#include <QString>
#include <QDateTime>

// AwsMock includes

/**
 * @brief Secrets counter
 *
 * @author jens.vogt\@opitz-consulting.com
 */
struct SecretCounter {

    /**
     * AWS region
     */
    QString region;

    /**
     * AWS ARN
     */
    QString arn;

    /**
     * Name
     */
    QString name;

    /**
     * SecretId
     */
    QString secretId;

    /**
     * Creation date
     */
    QDateTime createdDate;

    /**
     * Deleted date
     */
    QDateTime deletedDate;

    /**
     * Last access date
     */
    QDateTime lastAccessedDate;

    /**
     * Last changed date
     */
    QDateTime lastChangedDate;

    /**
     * Last rotation date
     */
    QDateTime lastRotatedDate;

    /**
     * Next rotation date
     */
    QDateTime nextRotatedDate;

    /**
     * Created timestamp
     */
    QDateTime created;

    /**
     * Modified timestamp
     */
    QDateTime modified;

    /**
     * Next rotation date
     */
    bool rotationEnabled = false;

    /**
     * Rotation lambda ARN
     */
    QString rotationLambdaARN;

    /**
     * Rotation rules
     */
    //RotationRules rotationRules;

    /**
     * Secret values
     */
    QString secretString;

    void FromJson(const QJsonObject &jsonObject) {
        region = jsonObject["region"].toString();
        name = jsonObject["secretName"].toString();
        arn = jsonObject["secretArn"].toString();
        secretId = jsonObject["secretId"].toString();
        secretString = jsonObject["secretString"].toString();
        createdDate = QDateTime::fromString(jsonObject["createdDate"].toString(), Qt::ISODate);
        deletedDate = QDateTime::fromString(jsonObject["deletedDate"].toString(), Qt::ISODate);
        lastAccessedDate = QDateTime::fromString(jsonObject["lastAccessedDate"].toString(), Qt::ISODate);
        lastChangedDate = QDateTime::fromString(jsonObject["lastChangedDate"].toString(), Qt::ISODate);
        lastRotatedDate = QDateTime::fromString(jsonObject["lastRotatedDate"].toString(), Qt::ISODate);
        nextRotatedDate = QDateTime::fromString(jsonObject["nextRotatedDate"].toString(), Qt::ISODate);
        rotationEnabled = jsonObject["RotationEnabled"].toBool();
        rotationLambdaARN = jsonObject["RotationLambdaARN"].toString();
        created = QDateTime::fromString(jsonObject["created"].toString(), Qt::ISODate);
        modified = QDateTime::fromString(jsonObject["modified"].toString(), Qt::ISODate);
    }

    QJsonObject ToJsonObject() const {
        QJsonObject jsonObject;
        jsonObject["region"] = region;
        jsonObject["secretName"] = name;
        jsonObject["secretArn"] = arn;
        jsonObject["secretId"] = secretId;
        jsonObject["secretString"] = secretString;
        jsonObject["createdDate"] = createdDate.toString(Qt::ISODate);
        jsonObject["deletedDate"] = deletedDate.toString(Qt::ISODate);
        jsonObject["lastAccessedDate"] = lastAccessedDate.toString(Qt::ISODate);
        jsonObject["lastRotatedDate"] = lastRotatedDate.toString(Qt::ISODate);
        jsonObject["nextRotatedDate"] = nextRotatedDate.toString(Qt::ISODate);
        jsonObject["rotationEnabled"] = rotationEnabled;
        jsonObject["rotationLambdaARN"] = rotationLambdaARN;
        jsonObject["created"] = created.toString(Qt::ISODate);
        jsonObject["modified"] = modified.toString(Qt::ISODate);
        return jsonObject;
    }
};

#endif//AWSMOCK_QT_UI_SECRETSMANAGER_SECRETSMANAGER_SECRET_H
