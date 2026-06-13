#pragma once

// Qt includes
#include <QElapsedTimer>
#include <QMessageBox>

// AwsMock includes
#include <dto/ssm/SSMParameterGetResponse.h>
#include <dto/ssm/SSMParameterListResponse.h>
#include <dto/ssm/SSMParameterPutResponse.h>
#include <utils/BaseService.h>
#include <utils/Configuration.h>
#include <utils/EventBus.h>
#include <utils/Logging.h>
#include <utils/RestManager.h>

class SSMService final : public BaseService {
    Q_OBJECT

public:
    /**
     * @brief SSM service constructor
     */
    SSMService() {
        setApis({"ListParameters", "CreateParameter", "GetParameter", "UpdateParameter", "DeleteParameter"});
    }

    /**
     * @brief Lists SSM parameters with specified filtering, pagination, and sorting options.
     *
     * This method sends a request to retrieve a list of SSM parameters based on the provided
     * filtering, pagination, and sorting criteria. When the response is received, it emits
     * the `ListParameterSignal` with the retrieved parameter list. In case of failure,
     * it logs the error and triggers a timing signal for monitoring.
     *
     * @param prefix The prefix used to filter the parameter list.
     * @param pageSize The number of parameters to include per page.
     * @param pageIndex The index of the page to retrieve.
     * @param sortAttribute The attribute to sort the parameter list by.
     * @param sortDirection The direction of sorting (e.g., ascending or descending).
     */
    void ListParameters(const QString &prefix, long pageSize, long pageIndex, const QString &sortAttribute, int sortDirection);

    /**
     * @brief Creates a new SSM parameter.
     *
     * This method sends a request to create a new parameter in the SSM service.
     * The parameter details, such as region, name, value, description, type, and optionally
     * a KMS key ARN, are provided as input. Upon successful creation, it triggers the
     * `ReloadParameterListSignal` to update the application. In case of an error,
     * the error is logged and a timing signal for monitoring is emitted.
     *
     * @param parameter The parameter details required to create the SSM parameter.
     */
    void CreateParameter(const SSMParameterCounter &parameter);

    /**
     * @brief Retrieves a specific parameter by name from the SSM service, including its description.
     *
     * This method sends an HTTP POST request to the SSM service to retrieve the specified parameter.
     * It emits a signal upon success or logs errors in case of failure.
     *
     * @param name The name of the parameter to retrieve.
     */
    void GetParameter(const QString &name);

    /**
     * @brief Updates an SSM parameter with the provided information and posts a request to the REST manager.
     *
     * This method constructs a structured JSON request for updating an SSM parameter
     * and sends it asynchronously using the REST manager. It processes the server
     * response and emits relevant signals based on the operation's success or failure.
     *
     * @param parameter The SSMParameterCounter object containing details such as
     *                  region, name, value, description, type, KMS key ARN, tags, and other metadata
     *                  required for updating the parameter.
     */
    void UpdateParameter(const SSMParameterCounter &parameter);

    /**
     * @brief Deletes an SSM parameter.
     *
     * This method sends a request to delete the specified parameter
     * from the SSM service. Upon successful completion, it triggers
     * the `ReloadParameterListSignal` to update the UI or other
     * application components. In case of failure, logs the error
     * and emits a timing signal for monitoring purposes.
     *
     * @param parameterName The name of the parameter to be deleted.
     */
    void DeleteParameter(const QString &parameterName);

signals:
    /**
     * @brief Emits a signal containing the response of an SSM parameter list request.
     *
     * This method activates the `ListParameterSignal` with the given response object,
     * allowing connected components to process or display the retrieved list of SSM parameters.
     *
     * @param parameterListResponse The response object containing the list of SSM parameters.
     */
    void ListParameterSignal(const SSMParameterListResponse &parameterListResponse);

    /**
     * @brief Emits a signal containing the response of an SSM parameter put operation.
     *
     * This method triggers the `PutParameterSignal` with the provided response object,
     * allowing connected components to handle the result of the parameter put operation.
     *
     * @param parameterPutResponse The response object representing the result of the SSM parameter put operation.
     */
    void PutParameterSignal(const SSMParameterPutResponse &parameterPutResponse);

    /**
     * @brief Activates the ReloadParameterListSignal signal.
     *
     * This method triggers the ReloadParameterListSignal signal by invoking
     * the QMetaObject activation mechanism.
     */
    void ReloadParameterListSignal();

    /**
     * @brief Emits a signal when a parameter is fetched.
     * @param parameterGetResponse The response object containing the fetched parameter details.
     */
    void GetParameterSignal(const SSMParameterGetResponse &parameterGetResponse);

private:
    /**
     * @brief HTTP REST manager
     */
    RestManager _restManager;
};
