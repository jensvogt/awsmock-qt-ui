//
// Created by vogje01 on 11/9/25.
//

#ifndef AWSMOCK_QT_UI_SQSMESSAGEADDDIALOG_H
#define AWSMOCK_QT_UI_SQSMESSAGEADDDIALOG_H

#include <utility>

#include <QDialog>

#include <modules/sqs/SQSMessageList.h>

QT_BEGIN_NAMESPACE

namespace Ui {
    class SQSMessageAddDialog;
}

QT_END_NAMESPACE

class SQSMessageAddDialog final : public QDialog {
    Q_OBJECT

public:
    explicit SQSMessageAddDialog(QWidget *parent = nullptr);

    ~SQSMessageAddDialog() override;

    void HandleAccept();

    void HandleReject();

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


#endif //AWSMOCK_QT_UI_SQSMESSAGEADDDIALOG_H
