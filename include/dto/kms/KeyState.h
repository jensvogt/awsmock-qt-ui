//
// Created by vogje01 on 11/25/23.
//

#ifndef AWSMOCK_QT_UI_KMS_KEY_STAT_H
#define AWSMOCK_QT_UI_KMS_KEY_STAT_H

// Qt includes
#include <QMap>
#include <QString>

/**
 * @brief KMS key state
 *
 * @author jens.vogt\@opitz-consulting.com
 */
enum class KeyState {
    CREATING,
    ENABLED,
    DISABLED,
    PENDING_DELETION,
    PENDING_IMPORT,
    PENDING_REPLICATION_DELETION,
    UNAVAILABLE,
    UPDATING
};

static std::map<KeyState, QString> KeyStateNames{
    {KeyState::CREATING, "Creating"},
    {KeyState::ENABLED, "Enabled"},
    {KeyState::DISABLED, "Disabled"},
    {KeyState::PENDING_DELETION, "PendingDeletion"},
    {KeyState::PENDING_IMPORT, "PendingImport"},
    {KeyState::PENDING_REPLICATION_DELETION, "PendingReplicaDeletion"},
    {KeyState::UNAVAILABLE, "Unavailable"},
    {KeyState::UPDATING, "Updating"}
};

[[maybe_unused]] static QString KeyStateToString(const KeyState &keyState) {
    return KeyStateNames[keyState];
}

[[maybe_unused]] static KeyState KeyStateFromString(const QString &keyState) {
    for (auto &[fst, snd]: KeyStateNames) {
        if (snd == keyState) {
            return fst;
        }
    }
    return KeyState::UNAVAILABLE;
}

#endif// AWSMOCK_QT_UI_KMS_KEY_STAT_H
