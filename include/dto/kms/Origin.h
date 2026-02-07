//
// Created by vogje01 on 11/25/23.
//

#ifndef AWSMOCK_QT_UI_KMS_ORIGIN_H
#define AWSMOCK_QT_UI_KMS_ORIGIN_H

// Qt includes
#include <QMap>
#include <QString>

/**
 * @brief KMS key origin
 *
 * @author jens.vogt\@opitz-consulting.com
 */
enum class Origin {
    AWS_KMS,
    EXTERNAL,
    AWS_CLOUDHSM,
    EXTERNAL_KEY_STORE,
    UNKNOWN
};

static std::map<Origin, QString> originNames{
    {Origin::AWS_KMS, "AWS_KMS"},
    {Origin::EXTERNAL, "EXTERNAL"},
    {Origin::AWS_CLOUDHSM, "AWS_CLOUDHSM"},
    {Origin::EXTERNAL_KEY_STORE, "EXTERNAL_KEY_STORE"},
};

[[maybe_unused]] static QString OriginToString(const Origin &origin) {
    return originNames[origin];
}

[[maybe_unused]] static Origin OriginFromString(const QString &origin) {
    for (auto &[fst, snd]: originNames) {
        if (snd == origin) {
            return fst;
        }
    }
    return Origin::AWS_KMS;
}

static QStringList GetOriginNames() {
    QStringList origins;
    for (auto &snd: originNames | std::views::values) {
        origins << snd;
    }
    return origins;
}

#endif// AWSMOCK_QT_UI_KMS_ORIGIN_H
