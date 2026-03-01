//
// Created by vogje01 on 11/25/23.
//

#ifndef AWSMOCK_QT_UI_KMS_KEY_SPEC_H
#define AWSMOCK_QT_UI_KMS_KEY_SPEC_H

// Qt includes
#include <QMap>
#include <QString>

/**
 * @brief KMS master key specification
 *
 * @author jens.vogt\@opitz-consulting.com
 */
enum class KeySpec {
    RSA_2048,
    RSA_3072,
    RSA_4096,
    ECC_NIST_P256,
    ECC_NIST_P384,
    ECC_NIST_P521,
    ECC_SECG_P256K1,
    SYMMETRIC_DEFAULT,
    HMAC_224,
    HMAC_256,
    HMAC_384,
    HMAC_512
};

static std::map<KeySpec, QString> keySpecNames{
    {KeySpec::RSA_2048, "RSA_2048"},
    {KeySpec::RSA_3072, "RSA_3072"},
    {KeySpec::RSA_4096, "RSA_4096"},
    {KeySpec::ECC_NIST_P256, "ECC_NIST_P256"},
    {KeySpec::ECC_NIST_P384, "ECC_NIST_P384"},
    {KeySpec::ECC_NIST_P521, "ECC_NIST_P521"},
    {KeySpec::ECC_SECG_P256K1, "ECC_SECG_P256K1"},
    {KeySpec::SYMMETRIC_DEFAULT, "SYMMETRIC_DEFAULT"},
    {KeySpec::HMAC_224, "HMAC_224"},
    {KeySpec::HMAC_256, "HMAC_256"},
    {KeySpec::HMAC_384, "HMAC_384"},
    {KeySpec::HMAC_512, "HMAC_512"}
};

[[maybe_unused]] static QString KeySpecToString(const KeySpec &keySpec) {
    return keySpecNames[keySpec];
}

[[maybe_unused]] static KeySpec KeySpecFromString(const QString &keySpec) {
    for (auto &[fst, snd]: keySpecNames) {
        if (snd == keySpec) {
            return fst;
        }
    }
    return KeySpec::SYMMETRIC_DEFAULT;
}

static QStringList GetKeySpecNames() {
    QStringList keyStates;
    for (auto &snd: keySpecNames | std::views::values) {
        keyStates << snd;
    }
    return keyStates;
}

#endif// AWSMOCK_QT_UI_KMS_KEY_SPEC_H
