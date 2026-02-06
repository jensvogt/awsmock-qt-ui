#ifndef AWSMOCK_QT_UI_KMS_SERVICE_H
#define AWSMOCK_QT_UI_KMS_SERVICE_H

// QT includes
#include <QMessageBox>
#include <QElapsedTimer>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>

// Awsmock includes
#include <utils/Configuration.h>
#include <utils/RestManager.h>
#include <utils/EventBus.h>
#include <utils/BaseService.h>
#include <dto/kms/KMSListKeysRequest.h>
#include <dto/kms/KMSListKeysResponse.h>

class KMSService final : public BaseService {
    Q_OBJECT

public:
    /**
     * @brief KMS service
     */
    KMSService() = default;

    /**
     * @brief List KMS keys
     *
     * @param prefix prefix for the KMS key ID
     */
    void ListKmsKeys(const QString &prefix = {});

    /**
     * @brief Deletes a KMS key
     *
     * @param keyId KMS key ID
     */
    void DeleteKey(const QString &keyId);

signals:
    /**
     * @brief Send when a new key list is available
     *
     * @param kmsKeyListResponse KMS key list response
     */
    void ListKeysSignal(const KMSListKeysResponse &kmsKeyListResponse);

    /**
     * @brief Send when a new key list is available
     */
    void ReloadKeySignal();

private:
    /**
     * @brief HTTP REST manager
     */
    RestManager _restManager;
};


#endif // AWSMOCK_QT_UI_KMS_SERVICE_H
