//
// Created by jensv on 05/05/2026.
//

#ifndef AWSMOCK_QT_UI_SNS_MESSAGE_WINDOW_H
#define AWSMOCK_QT_UI_SNS_MESSAGE_WINDOW_H

// Qt includes
#include <QDialog>
#include <QDialogButtonBox>

// Awsmock includes
#include <modules/sns/SNSService.h>
#include <utils/BaseDialog.h>
#include <utils/IconUtils.h>

QT_BEGIN_NAMESPACE

namespace Ui {
    class SNSMessageWindow;
}

QT_END_NAMESPACE

class SNSMessageWindow : public BaseDialog {
    Q_OBJECT

public:
    /**
     * @brief Constructor
     *
     * @param messageId SQS message ID
     * @param parent parent widget
     */
    explicit SNSMessageWindow(const QString &messageId, QWidget *parent = nullptr);

    /**
     * @brief Destructor
     */
    ~SNSMessageWindow() override;

    /**
     * @brief Load dialog content
     */
    void LoadContent() override;

    /**
     * @brief Set status label
     */
    void SetLastUpdate() const;

    /**
     * @brief Update dialog content
     */
    void UpdateContent(const SNSGetMessageDetailsResponse &response) const;

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
    Ui::SNSMessageWindow *_ui{};

    /**
     * @brief SNS service
     */
    SNSService *_snsService{};

    /**
     * @brief SQS message ID
     */
    QString _messageId;
};


#endif // AWSMOCK_QT_UI_SNS_MESSAGE_WINDOW_H
