//
// Created by vogje01 on 2/21/26.
//

#ifndef AWSMOCK_QT_UI_S3_FILTER_RULE_H
#define AWSMOCK_QT_UI_S3_FILTER_RULE_H

// Qt includes
#include <QMap>

/**
 * @brief S3 name type for the filter rules
 *
 * @author jens.vogt\@opitz-consulting.com
 */
enum class S3FilterNameType {
    prefix,
    suffix
};

static QMap<S3FilterNameType, QString> S3FilterNameNames{
    {S3FilterNameType::prefix, "prefix"},
    {S3FilterNameType::suffix, "suffix"}
};

[[maybe_unused]] static QString S3FilterNameToString(const S3FilterNameType &nameType) {
    return S3FilterNameNames[nameType];
}

[[maybe_unused]] static S3FilterNameType S3FilterNameFromString(const QString &nameType) {
    return S3FilterNameNames.key(nameType, S3FilterNameType::prefix);
}

[[maybe_unused]] static QStringList S3FilterRuleNameList() {
    QStringList filterRuleTypeNames;
    for (auto &name: S3FilterNameNames.values()) {
        filterRuleTypeNames.append(name);
    }
    return filterRuleTypeNames;
}

struct S3FilterRule {

    /**
     * Name
     */
    S3FilterNameType name = S3FilterNameType::prefix;

    /**
     * Value
     */
    QString filterValue;

    void FromJson(const QJsonObject &jsonObject) {
        name = S3FilterNameFromString(jsonObject["name"].toString());
        filterValue = jsonObject["filterValue"].toString();
    }

};

#endif //AWSMOCK_QT_UI_S3_FILTER_RULE_H
