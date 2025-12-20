#ifndef SNS_TOPIC_DETAILS_DIALOG_H
#define SNS_TOPIC_DETAILS_DIALOG_H

// Qt includes
#include <QDialog>

// AwsMock includes
#include <utils/BaseDialog.h>
#include <utils/PrefixFilterModel.h>
#include <utils/IconUtils.h>
#include <utils/DateTimeUtils.h>
#include <modules/sns/SNSService.h>

namespace Ui {
    class SNSTopicDetailsDialog;
}

class SNSTopicDetailsDialog : public BaseDialog {
    Q_OBJECT

public:
    /**
     * @brief Constructor
     *
     * @param topicArn AWS topic ARN
     * @param parent parent widget
     */
    explicit SNSTopicDetailsDialog(const QString &topicArn, QWidget *parent = nullptr);

    /**
     * @brief Destructor
     */
    ~SNSTopicDetailsDialog() override;

    /**
     * @brief Load dialog content
     */
    void LoadContent() override;

    /**
     * @brief Handle OK button
     */
    void HandleAccept();

    /**
     * @brief Handle CANCEL button
     */
    void HandleReject();

private:
    /**
     * @brief Update topic
     *
     * @param response topic response
     */
    void UpdateTopicDetails(const SNSGetTopicDetailsResponse &response) const;

    /**
     * @brief Handle attribute table updates
     *
     * @param response attribute list response
     */
    void UpdateTopicAttributes(const ListTopicAttributesResponse &response) const;

    void UpdateTopicTags(const ListTopicTagsResponse &response) const;

    /**
     * @brief Current tab changed callback, used for lazy loading.
     *
     * @param index current tab index
     */
    void CurrentTabChanged(int index) const;

    /**
     * @brief Setup attributes table
     */
    void SetupAttributesTable();

    /**
     * @brief Setup tags table
     */
    void SetupTagsTable();

    /**
     * @brief Setup subscription table
     */
    void SetupSubscriptionsTable();

    void UpdateTopicSubscriptions(const ListTopicSubscriptionsResponse &response) const;

    /**
     * @brief UI components
     */
    Ui::SNSTopicDetailsDialog *_ui;

    /**
     * @brief Topic AWS ARN
     */
    QString topicArn;

    /**
     * @brief SNS REST service
     */
    SNSService *_snsService;

    /**
     * @brief Changed flag
     */
    bool changed = false;

    /**
     *  @brief Attributes table data model
     */
    QStandardItemModel *_attributesDataModel{};

    /**
     * @brief Attributes data proxy model
     */
    PrefixFilterProxyModel *_attributesProxyModel{};

    /**
     * @brief Sort column index for attributes
     */
    int _attributesSortColumn = 0;

    /**
     * @brief Sort order attributes
     */
    Qt::SortOrder _attributesSortOrder = Qt::AscendingOrder;

    /**
     *  @brief Tags table data model
     */
    QStandardItemModel *_tagsDataModel{};

    /**
     * @brief Tags data proxy model
     */
    PrefixFilterProxyModel *_tagsProxyModel{};

    /**
     * @brief Sort column index for tags
     */
    int _tagsSortColumn = 0;

    /**
     * @brief Sort order tags
     */
    Qt::SortOrder _tagsSortOrder = Qt::AscendingOrder;

    /**
     *  @brief Subscriptions table data model
     */
    QStandardItemModel *_subscriptionsDataModel{};

    /**
     * @brief Subscriptions data proxy model
     */
    PrefixFilterProxyModel *_subscriptionsProxyModel{};

    /**
     * @brief Sort column index for subscriptions
     */
    int _subscriptionsSortColumn = 0;

    /**
     * @brief Sort order subscriptions
     */
    Qt::SortOrder _subscriptionsSortOrder = Qt::AscendingOrder;
};

#endif // SNS_TOPIC_DETAILS_DIALOG_H
