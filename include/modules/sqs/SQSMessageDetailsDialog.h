#ifndef SQS_MESSAGE_DETAILS_DIALOG_H
#define SQS_MESSAGE_DETAILS_DIALOG_H

#include <QDialog>

#include <modules/sqs/SQSService.h>
#include <utils/WordHIghLighter.h>
#include <utils/IconUtils.h>

#include "SQSMessageList.h"

namespace Ui {
    class SQSMessageDetailsDialog;
}

class SQSMessageDetailsDialog : public QDialog {
    Q_OBJECT

public:
    /**
     * @brief Constructor
     *
     * @param messageId SQS message ID
     * @param parent parent widget
     */
    explicit SQSMessageDetailsDialog(const QString &messageId, QWidget *parent = nullptr);


    /**
     * @brief Destructor
     */
    ~SQSMessageDetailsDialog() override;

private:
    /**
     * @brief Update UI components
     *
     * @param response SQS REST response
     */
    void UpdateMessageDetails(const SQSGetMessageDetailsResponse &response);

    void SaveToFile() const;

    static QString SelectFilename();

    /**
     * @brief UI components
     */
    Ui::SQSMessageDetailsDialog *_ui;

    /**
     * @brief SQS message ID
     */
    QString _messageId;

    /**
     * @brief SQS REST service
     */
    SQSService *_sqsService;

    /**
     * @brief Changed flag
     */
    bool _changed = false;

    /**
     * @brief Local copy of REST response
     */
    SQSGetMessageDetailsResponse _response;
};

#endif // SQS_MESSAGE_DETAILS_DIALOG_H
