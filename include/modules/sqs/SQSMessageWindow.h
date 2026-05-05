//
// Created by jensv on 05/05/2026.
//

#ifndef AWSMOCK_QT_UI_SQSMESSAGEWINDOW_H
#define AWSMOCK_QT_UI_SQSMESSAGEWINDOW_H

// Qt includes
#include <QDialog>
#include <QDialogButtonBox>

// Awsmock includes
#include <modules/sqs/SQSService.h>
#include <utils/BaseDialog.h>

QT_BEGIN_NAMESPACE

namespace Ui {
    class SQSMessageWindow;
}

QT_END_NAMESPACE

class SQSMessageWindow : public BaseDialog {
    Q_OBJECT

public:
    /**
     * @brief Constructor
     *
     * @param messageId SQS message ID
     * @param parent parent widget
     */
    explicit SQSMessageWindow(const QString &messageId, QWidget *parent = nullptr);

    /**
     * @brief Load dialog content
     */
    void LoadContent() override;

    void SetLastUpdate() const;

    /**
     * @brief Update dialog content
     */
    void UpdateContent(const SQSGetMessageDetailsResponse &response) const;

    /**
     * @brief Destructor
     */
    ~SQSMessageWindow() override;

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
     * @brief UI components
     */
    Ui::SQSMessageWindow *_ui{};

    /**
     * @brief SQS service
     */
    SQSService *_sqsService{};

    /**
     * @brief SQS message ID
     */
    QString _messageId;
};


#endif //AWSMOCK_QT_UI_SQSMESSAGEWINDOW_H
