//
// Created by vogje01 on 12/19/25.
//

#pragma once

// Qt includes
#include <QString>
#include <QDateTime>

/**
 * @brief SSM parameter type
 *
 * @author jens.vogt\@opitz-consulting.com
 */
enum class ParameterType {
    string,
    stringList,
    secureString
};

static std::map<ParameterType, QString> ParameterTypeNames{
    {ParameterType::string, "String"},
    {ParameterType::stringList, "StringList"},
    {ParameterType::secureString, "SecureString"}
};

[[maybe_unused]] static QString ParameterTypeToString(const ParameterType &parameterType) {
    return ParameterTypeNames[parameterType];
}

[[maybe_unused]] static ParameterType ParameterTypeFromString(const QString &parameterType) {
    for (auto &[fst, snd]: ParameterTypeNames) {
        if (snd == parameterType) {
            return fst;
        }
    }
    return ParameterType::string;
}

struct SSMParameterCounter {

    /**
     * AWS region
     */
    QString region;

    /**
     * Parameter name
     */
    QString name;

    /**
     * Parameter value
     */
    QString parameterValue;

    /**
     * Description
     */
    QString description;

    /**
     * Parameter contentType
     */
    ParameterType type = ParameterType::string;

    /**
     * KMS key ARN
     */
    QString kmsKeyArn;

    /**
     * Parameter tier
     */
    QString tier;

    /**
     * AWS ARN
     */
    QString arn;

    /**
     * Tags
     */
    QMap<QString, QString> tags;

    /**
     * Created timestamp
     */
    QDateTime created;

    /**
     * Modified timestamp
     */
    QDateTime modified;

    void FromJson(const QJsonObject &jsonObject) {
        region = jsonObject["Region"].toString();
        name = jsonObject["Name"].toString();
        parameterValue = jsonObject["Value"].toString();
        description = jsonObject["Description"].toString();
        type = ParameterTypeFromString(jsonObject["Type"].toString());
        kmsKeyArn = jsonObject["KmsKeyArn"].toString();
        tier = jsonObject["Tier"].toString();
        arn = jsonObject["ARN"].toString();
        created = QDateTime::fromString(jsonObject["Created"].toString(), Qt::ISODate);
        modified = QDateTime::fromString(jsonObject["Modified"].toString(), Qt::ISODate);

        if (jsonObject.contains("Tags") && jsonObject["Tags"].isObject()) {
            for (const auto &key: jsonObject["Tags"].toObject().keys()) {
                tags[key] = jsonObject["Tags"].toObject()[key].toString();
            }
        }
    }
};
