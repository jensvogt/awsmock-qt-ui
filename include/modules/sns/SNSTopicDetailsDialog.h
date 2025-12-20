#ifndef SNS_TOPIC_DETAILS_DIALOG_H
#define SNS_TOPIC_DETAILS_DIALOG_H

// Qt includes
#include <QDialog>

// AwsMock includes
#include <utils/BaseDialog.h>
#include <utils/PrefixFilterModel.h>
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
    void UpdateTopicAttributes(const ListTopicAttributesCountersResponse &response) const;

    /**
     * @brief Current tab changed callback, used for lazy loading.
     *
     * @param index current tab index
     */
    void CurrentTabChanged(int index) const;

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
};

#endif // SNS_TOPIC_DETAILS_DIALOG_H
