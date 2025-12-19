#ifndef AWSMOCK_QT_UI_SSM_SERVICE_H
#define AWSMOCK_QT_UI_SSM_SERVICE_H

// Qt includes
#include <QMessageBox>
#include <QObject>
#include <QUrlQuery>
#include <QElapsedTimer>

// AwsMock includes
#include <utils/Configuration.h>
#include <utils/RestManager.h>
#include <utils/BaseService.h>
#include <utils/EventBus.h>
#include <dto/ssm/SSMParameterListResponse.h>
#include <dto/ssm/SSMParameterGetResponse.h>

class SSMService final : public BaseService {
    Q_OBJECT

public:
    /**
     * @brief SSM service constructor
     */
    SSMService() = default;

    /**
     * @brief List SSM parameters
     *
     * @param prefix topic name prefix
     * @param sortColumn
     * @param sortDirection
     * @param sortColumn
     * @param sortDirection
     */
    void ListParameters(const QString &prefix, int sortColumn, int sortDirection);

    void GetParameter(const QString &name);

    void UpdateParameter(const SSMParameterCounter &parameter);

signals:
    /**
     * @brief Signaled when a parameter list is available
     *
     * @param parameterListResponse list of parameters
     */
    void ListParameterSignal(const SSMParameterListResponse &parameterListResponse);

    /**
     * @brief Signaled when a bucket list needs to be reloaded
     *
     */
    void ReloadParameterListSignal();

    /**
     * @brief Signaled when a parameter is available
     *
     * @param parameterGetResponse parameter counter
     */
    void GetParameterSignal(const SSMParameterGetResponse &parameterGetResponse);

private:
    /**
     * @brief HTTP REST manager
     */
    RestManager _restManager;
};


#endif // AWSMOCK_QT_UI_SSM_SERVICE_H
