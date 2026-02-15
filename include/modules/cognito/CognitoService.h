#ifndef AWSMOCK_QT_UI_COGNITO_SERVICE_H
#define AWSMOCK_QT_UI_COGNITO_SERVICE_H

// Qt includes
#include <QMessageBox>
#include <QElapsedTimer>

// AwsMock includes
#include <utils/Logging.h>
#include <utils/Configuration.h>
#include <utils/RestManager.h>
#include <utils/BaseService.h>
#include <dto/cognito/CognitoUserpoolListResponse.h>

class CognitoService final : public BaseService {
    Q_OBJECT

public:
    /**
     * @brief Cognito service
     */
    CognitoService() = default;

    /**
     * @brief List user pools
     *
     * @param prefix table name prefix
     * @param pageSize
     * @param pageIndex
     * @param sortColumn
     * @param sortDirection
     * @param pageSize
     * @param pageIndex
     * @param sortColumn
     * @param sortDirection
     */
    void ListUserpools(const QString &prefix, long pageSize, long pageIndex, const QString &sortColumn, int sortDirection);

signals:
    /**
     * @brief Signaled when a userpool list is available
     *
     * @param listUserpoolResponse list of user pools
     */
    void ListUserPoolsSignal(const CognitoUserpoolListResponse &listUserpoolResponse);

private:
    /**
     * @brief HTTP REST manager
     */
    RestManager _restManager;
};


#endif // AWSMOCK_QT_UI_COGNITO_SERVICE_H
