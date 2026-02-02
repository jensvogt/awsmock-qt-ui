#ifndef AWSMOCK_QT_UI_SSM_SERVICE_H
#define AWSMOCK_QT_UI_SSM_SERVICE_H

// Qt includes
#include <QMessageBox>
#include <QElapsedTimer>

// AwsMock includes
#include <utils/Configuration.h>
#include <utils/RestManager.h>
#include <utils/BaseService.h>
#include <utils/EventBus.h>
#include <dto/ssm/SSMParameterListResponse.h>
#include <dto/ssm/SSMParameterGetResponse.h>
#include <dto/ssm/SSMParameterPutResponse.h>

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

    /**
     * @brief Adds a new SSM parameters
     *
     * @param parameter parameter counter DTO
     */
    void CreateParameter(const SSMParameterCounter &parameter);

    /**
     * @brief Get details of a parameter
     *
     * @param name parameter name
     */
    void GetParameter(const QString &name);

    /**
     * @brief Update an existing parameter
     *
     * @param parameter parameter counter
     */
    void UpdateParameter(const SSMParameterCounter &parameter);

    /**
     * @brief Delete an existing parameter
     *
     * @param parameterName parameter name
     */
    void DeleteParameter(const QString &parameterName);

signals:
    /**
     * @brief Signaled when a parameter list is available
     *
     * @param parameterListResponse list of parameters
     */
    void ListParameterSignal(const SSMParameterListResponse &parameterListResponse);

    /**
     * @brief Signaled when a parameter create response available
     *
     * @param parameterPutResponse parameter put response DTO
     */
    void PutParameterSignal(const SSMParameterPutResponse &parameterPutResponse);

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
