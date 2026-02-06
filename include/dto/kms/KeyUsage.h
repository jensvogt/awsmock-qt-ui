//
// Created by vogje01 on 4/26/24.
//
#ifndef AWSMOCK_QT_UI_KMS_KEY_USAGE_H
#define AWSMOCK_QT_UI_KMS_KEY_USAGE_H

// Qt includes
#include <QMap>
#include <QString>

/**
 * @brief KMS key usage
 *
 * @author jens.vogt\@opitz-consulting.com
 */
enum class KeyUsage {
    SIGN_VERIFY,
    ENCRYPT_DECRYPT,
    GENERATE_VERIFY_MAC
};

static std::map<KeyUsage, QString> KeyUsageNames{
    {KeyUsage::SIGN_VERIFY, "SIGN_VERIFY"},
    {KeyUsage::ENCRYPT_DECRYPT, "ENCRYPT_DECRYPT"},
    {KeyUsage::GENERATE_VERIFY_MAC, "GENERATE_VERIFY_MAC"}
};

[[maybe_unused]] static QString KeyUsageToString(const KeyUsage &keyUsage) {
    return KeyUsageNames[keyUsage];
}

[[maybe_unused]] static KeyUsage KeyUsageFromString(const QString &keyUsage) {
    for (auto &[fst, snd]: KeyUsageNames) {
        if (snd == keyUsage) {
            return fst;
        }
    }
    return KeyUsage::ENCRYPT_DECRYPT;
}

#endif// AWSMOCK_QT_UI_KMS_KEY_USAGE_H
