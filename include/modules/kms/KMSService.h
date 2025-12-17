#ifndef AWSMOCK_QT_UI_KMS_SERVICE_H
#define AWSMOCK_QT_UI_KMS_SERVICE_H

// QT includes
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QObject>
#include <QElapsedTimer>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>

// Awsmock Qt includes
#include <utils/Configuration.h>
#include <utils/RestManager.h>
#include <utils/EventBus.h>
#include <utils/BaseService.h>
#include <dto/kms/KMSKeyListResponse.h>

class KMSService final : public BaseService {
    Q_OBJECT

public:
    /**
     * @brief KMS service
     */
    KMSService() = default;

    /**
     * @brief List KMS keys
     */
    void ListKmsKeys();

signals:
    void ListKeysSignal(const KMSKeyListResponse &kmsKeyListResponse);

private:
    /**
     * @brief HTTP REST manager
     */
    RestManager _restManager;
};


#endif // AWSMOCK_QT_UI_KMS_SERVICE_H
