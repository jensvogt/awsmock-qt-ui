//
// Created by vogje01 on 11/9/25.
//

#ifndef AWSMOCK_QT_UI_SQS_MESSAGE_ADD_DIALOG_H
#define AWSMOCK_QT_UI_SQS_MESSAGE_ADD_DIALOG_H

#include <utility>

#include <QDialog>
#include <QFileDialog>

#include <utils/BaseDialog.h>
#include <utils/IconUtils.h>
#include <modules/sqs/SQSMessageList.h>

QT_BEGIN_NAMESPACE

namespace Ui {
    class SQSMessageAddDialog;
}

QT_END_NAMESPACE

class SQSMessageAddDialog final : public BaseDialog {
    Q_OBJECT

public:
    explicit SQSMessageAddDialog(QWidget *parent = nullptr);

    ~SQSMessageAddDialog() override;

    void HandleAccept();

    void HandleReject();

    void HandleBrowseButton() const;

    void HandlePrettyButton(bool checked) const;

    void HandleAddAttributeButton() const;

private:
    /**
     * @brief UI components
     */
    Ui::SQSMessageAddDialog *_ui;

    /**
     * @brief SQS REST service
     */
    SQSService *_sqsService{};
};


#endif //AWSMOCK_QT_UI_SQS_MESSAGE_ADD_DIALOG_H
