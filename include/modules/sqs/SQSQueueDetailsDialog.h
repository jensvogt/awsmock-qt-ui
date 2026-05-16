#ifndef AWSMOCK_QT_UI_SQS_QUEUE_DETAILS_DIALOG_H
#define AWSMOCK_QT_UI_SQS_QUEUE_DETAILS_DIALOG_H

#include <QMenu>

#include <dto/sqs/SQSListQueueDefaultAttribtesResponse.h>
#include <dto/sqs/SQSListQueueTagsResponse.h>
#include <dto/sqs/SQSUpdateTagsRequest.h>
#include <modules/sqs/SQSQueueDefaultAttributeDialog.h>
#include <modules/sqs/SQSQueueTagsDialog.h>
#include <modules/sqs/SQSService.h>
#include <utils/BaseDialog.h>
#include <utils/IconUtils.h>
#include <utils/StringUtils.h>

namespace Ui {
    class SQSQueueDetailsDialog;
}

class SQSQueueDetailsDialog final : public BaseDialog {
    Q_OBJECT

public:
    explicit SQSQueueDetailsDialog(const QString &queueArn, QWidget *parent = nullptr);

    ~SQSQueueDetailsDialog() override;

    void LoadContent() override {
    };

private:
    /**
     * @brief Update SQS queue details
     *
     * @param response update queue details response DTO
     */
    void UpdateQueueDetails(const SQSGetQueueDetailsResponse &response);

    void HandleAccept();

    void HandleReject();

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

    void SetupDefaultAttributesTab();

    void UpdateDefaultAttributes(const SQSListQueueDefaultAttributesResponse &response) const;

    void ShowDefaultAttributeContextMenu(const QPoint &pos) const;

    void SetupTagsTab();

    void UpdateTags(const SQSListQueueTagsResponse &response) const;

    void SetupMainQueuesTab();

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
     * @brief Queue URL
     */
    QString _queueUrl;

    /**
     * @brief DLQ flag
     */
    bool _isDlq;

    /**
     * @brief List of main queues
     */
    QList<QString> _mainQueues;

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
     * @brief Default attribute sort column index
     *
     * @par Default sort column is 'key', index=0
     */
    int _defaultAttributeSortColumn = 0;

    /**
     * @brief Default attribute sort order
     */
    Qt::SortOrder _defaultAttributeSortOrder = Qt::DescendingOrder;

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

    /**
     * @brief Default attributes table model
     */
    QStandardItemModel *_defaultAttributesModel{};

    /**
     * @brief Tags table model
     */
    QStandardItemModel *_tagsModel{};
};

#endif // AWSMOCK_QT_UI_SQS_QUEUE_DETAILS_DIALOG_H
