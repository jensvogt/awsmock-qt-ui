#ifndef AWSMOCK_QT_UI_SQS_QUEUE_DETAILS_DIALOG_H
#define AWSMOCK_QT_UI_SQS_QUEUE_DETAILS_DIALOG_H

#include <modules/sqs/SQSService.h>
#include <utils/BaseDialog.h>

namespace Ui {
    class SQSQueueDetailsDialog;
}

class SQSQueueDetailsDialog final : public BaseDialog {
    Q_OBJECT

public:
    explicit SQSQueueDetailsDialog(const QString &queueArn, QWidget *parent = nullptr);

    ~SQSQueueDetailsDialog() override;

private slots:
    void on_sqsQueueDetailsButtonBox_accepted();

    void on_sqsQueueDetailsButtonBox_rejected();

private:
    /**
     * @brief Update SQS queue details
     *
     * @param response update queue details response DTO
     */
    void UpdateQueueDetails(const SQSGetQueueDetailsResponse &response);

    /**
     * @brief Setup of the attributes tab
     */
    void SetupAttributesTab() const;

    /**
     * @brief Update the SQS queue attributes table
     *
     * @param response attributes list response DTO
     */
    void UpdateQueueAttributes(const SQSQueueAttributeListResponse &response) const;

    /**
    * @brief Setup of the lambda triggers tab
    */
    void SetupLambdaTriggersTab() const;

    /**
     * @brief Update the lambda trigger tab
     *
     * @param response list lambda triggers response DTO
     */
    void UpdateQueueLambdaTriggers(const SQSListQueueLambdaTriggersResponse &response) const;

    /**
     * @brief UI components
     */
    Ui::SQSQueueDetailsDialog *_ui;

    /**
     * @brief Queue ARN
     */
    QString _queueArn;

    /**
     * @brief SQS REST service
     */
    SQSService *_sqsService;

    /**
     * @brief changed flag
     */
    bool changed = false;

    /**
     * @brief Attributes sort column index
     *
     * @par Default sort column is 'arn', index=1
     */
    int _attributesSortColumn = 1;

    /**
     * @brief Attributes sort order
     */
    Qt::SortOrder _attributesSortOrder = Qt::DescendingOrder;

    /**
     * @brief Lambda trigger sort column index
     *
     * @par Default sort column is 'arn', index=1
     */
    int _lambdaTriggerSortColumn = 1;

    /**
     * @brief Lambda trigger sort order
     */
    Qt::SortOrder _lambdaTriggerSortOrder = Qt::DescendingOrder;

    /**
     * @brief Tags sort column index
     *
     * @par Default sort column is 'arn', index=1
     */
    int _tagsSortColumn = 1;

    /**
     * @brief Tags sort order
     */
    Qt::SortOrder _tagsSortOrder = Qt::DescendingOrder;
};

#endif // AWSMOCK_QT_UI_SQS_QUEUE_DETAILS_DIALOG_H
