#ifndef AWSMOCK_QT_UI_KMS_SERVICE_H
#define AWSMOCK_QT_UI_KMS_SERVICE_H

// QT includes
#include <QMessageBox>
#include <QElapsedTimer>

// Awsmock includes
#include <utils/Configuration.h>
#include <utils/RestManager.h>
#include <utils/EventBus.h>
#include <utils/BaseService.h>
#include <dto/kms/KMSListKeysRequest.h>
#include <dto/kms/KMSListKeysResponse.h>
#include <dto/kms/KMSGetKeyCounterResponse.h>
#include <dto/kms/KMSUpdateKeyCounterRequest.h>

struct KMSGetKeyCounterResponse;

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
     * @brief Gets a key counter
     *
     * @param keyId KMS key ID
     */
    void GetKeyCounter(const QString &keyId);

    /**
     * @brief Update an existing key couter
     *
     * @param request update request
     */
    void UpdateKeyCounter(const KMSUpdateKeyCounterRequest &request);

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
     * @brief Send when a new key counter is available
     *
     * @param getKeyCounterResponse KMS get key response
     */
    void GetKeyCounterSignal(const KMSGetKeyCounterResponse &getKeyCounterResponse);

    /**
     * @brief Send when a new key list is available
     */
    void ReloadKeysSignal();

private:
    /**
     * @brief HTTP REST manager
     */
    RestManager _restManager;
};


#endif // AWSMOCK_QT_UI_KMS_SERVICE_H
