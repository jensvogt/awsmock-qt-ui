//
// Created by vogje01 on 12/19/25.
//

#ifndef AWSMOCK_QT_UI_SSM_PARAMETER_EDIT_DIALOG_H
#define AWSMOCK_QT_UI_SSM_PARAMETER_EDIT_DIALOG_H

// C++ includes
#include <utility>

// Qt includes
#include <QDialog>
#include <QClipboard>
#include <QInputDialog>

// AwsMock includes
#include <utils/BaseDialog.h>
#include <utils/IconUtils.h>
#include <utils/PrefixFilterModel.h>
#include <utils/DateTimeUtils.h>
#include <modules/ssm/SSMService.h>
#include <modules/kms/KMSService.h>
#include <modules/ssm/SSMParameterAddTagDialog.h>

QT_BEGIN_NAMESPACE

namespace Ui {
    class SSMParameterEditDialog;
}

QT_END_NAMESPACE

class SSMParameterEditDialog : public BaseDialog {
    Q_OBJECT

public:
    /**
     * @brief Constructor
     *
     * @param parameterName name of the parameter
     * @param parent parent widget
     */
    explicit SSMParameterEditDialog(QString parameterName, QWidget *parent = nullptr);

    /**
     * @brief Destructor
     */
    ~SSMParameterEditDialog() override;

    /**
     * @brief Handle ok button
     */
    void HandleAccept();

    /**
     * @brief Handle cancel button
     */
    void HandleReject();

    /**
     * @biref load dialog content
     */
    void LoadContent() override;

    /**
     * @brief Get response callback
     *
     * @param parameterGetResponse parameter get respone
     */
    void HandleParameterGetSignal(const SSMParameterGetResponse &parameterGetResponse);

    /**
     * @brief Setup tabs widget
     */
    void SetupTagsTab();

private:
    /**
     *  @brief UI components
     */
    Ui::SSMParameterEditDialog *_ui;

    /**
     * @brief Parameter name
     */
    QString _parameterName;

    /**
     * @brief SSM REST service handler
     */
    SSMService *_ssmService;

    /**
     *  @brief Table data model
     */
    QStandardItemModel *_tagsDataModel{};

    /**
     * @brief Data proxy model
     */
    PrefixFilterProxyModel *_tagsProxyModel{};

    /**
     * @brief Changed flag
     */
    bool _changed = false;

    /**
     * @brief Backup copy of the parameter
     */
    SSMParameterCounter _parameter;
};


#endif // AWSMOCK_QT_UI_SSM_PARAMETER_EDIT_DIALOG_H
