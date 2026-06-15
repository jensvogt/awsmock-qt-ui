#pragma once

// Qt includes
#include <QMessageBox>
#include <QElapsedTimer>

// AwsMock includes
#include <utils/Logging.h>
#include <utils/Configuration.h>
#include <utils/RestManager.h>
#include <utils/BaseService.h>
#include <dto/cognito/CognitoUserpoolListResponse.h>
#include <dto/cognito/CognitoAddUserpoolResponse.h>
#include <dto/cognito/CognitoAddUserpoolRequest.h>
#include <dto/cognito/CognitoDeleteUserpoolRequest.h>

class CognitoService final : public BaseService {
    Q_OBJECT

public:
    /**
     * @brief Cognito service
     */
    CognitoService() {
        setApis({"ListUserpools", "CreateUserpool", "DeleteUserpool"});
    }

    /**
     * @brief List user pools
     *
     * @param prefix table name prefix
     * @param pageSize page size
     * @param pageIndex page index
     * @param sortColumn sort column name
     * @param sortDirection sort direction
     */
    void ListUserpools(const QString &prefix, long pageSize, long pageIndex, const QString &sortColumn, int sortDirection);

    /**
     * @brief Create a user pool
     *
     * @param request create userpool request
     */
    void CreateUserpool(const CognitoAddUserpoolRequest &request);

    /**
     * @brief Delete user pools
     *
     * @param request delete userpool request
     */
    void DeleteUserpool(const CognitoDeleteUserpoolRequest &request);

signals:
    /**
     * @brief Signaled when a userpool list is available
     *
     * @param listUserpoolResponse list of user pools
     */
    void ListUserPoolsSignal(const CognitoUserpoolListResponse &listUserpoolResponse);

    /**
     * @brief Signaled when the content should be refreshed
     */
    void RefreshContentSignal();

private:
    /**
     * @brief HTTP REST manager
     */
    RestManager _restManager;
};
