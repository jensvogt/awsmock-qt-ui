//
// Created by vogje01 on 12/19/25.
//

#ifndef AWSMOCK_QT_UI_SSM_PARAMETER_DIALOG_H
#define AWSMOCK_QT_UI_SSM_PARAMETER_DIALOG_H

// Qt includes
#include <QDialog>

// AwsMock includes
#include <utils/BaseDialog.h>
#include <utils/IconUtils.h>
#include <modules/ssm/SSMService.h>

#include "utils/PrefixFilterModel.h"

QT_BEGIN_NAMESPACE

namespace Ui {
    class SSMParameterDialog;
}

QT_END_NAMESPACE

class SSMParameterDialog : public BaseDialog {
    Q_OBJECT

public:
    explicit SSMParameterDialog(const QString &parameterName, QWidget *parent = nullptr);

    ~SSMParameterDialog() override;

    void HandleAccept();

    void HandleReject();

    void LoadContent() override;

    void HandleParameterGetSignal(const SSMParameterGetResponse &parameterGetResponse);

    void SetupTagsTab();

private:
    /**
     *  @brief UI components
     */
    Ui::SSMParameterDialog *_ui;

    /**
     * @brief Parameter name
     */
    QString _parameterName;

    /**
     * @brief REST service handler
     */
    SSMService *_ssmService;

    /**
     *  @brief Table data model
     */
    QStandardItemModel *_tagsDataModel;

    /**
     * @brief Data proxy model
     */
    PrefixFilterProxyModel *_tagsProxyModel;

    /**
     * @brief Changed flag
     */
    bool _changed = false;

    /**
     * @brief Backup copy of the parameter
     */
    SSMParameterCounter _parameter;
};


#endif //AWSMOCK_QT_UI_SSM_PARAMETER_DIALOG_H
