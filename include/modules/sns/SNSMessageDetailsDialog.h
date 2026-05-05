#ifndef AWSMOCK_QT_UI_SNS_MESSAGE_DETAILS_DIALOG_H
#define AWSMOCK_QT_UI_SNS_MESSAGE_DETAILS_DIALOG_H

// Qt includes
#include <QDialog>
#include <QHeaderView>
#include <QStandardItemModel>

// Awsmock includes
#include <modules/sns/SNSMessageWindow.h>
#include <modules/sns/SNSService.h>
#include <utils/DateTimeUtils.h>
#include <utils/IconUtils.h>
#include <utils/StringUtils.h>

namespace Ui {
    class SNSMessageDetailsDialog;
}

class SNSMessageDetailsDialog final : public QDialog {
    Q_OBJECT

public:
    /**
     * @brief Constructor
     *
     * @param messageId SNS message ID
     * @param parent
     */
    explicit SNSMessageDetailsDialog(const QString &messageId, QWidget *parent = nullptr);

    /**
     * @brief Destructor
     */
    ~SNSMessageDetailsDialog() override;

private slots:
    void on_prettyPushButton_toggled(bool checked) const;

    void SaveToFile() const;
    
    void HandleAccept();

    void HandleReject();

private:
    static QString SelectFilename();

    void UpdateMessageDetails(const SNSGetMessageDetailsResponse &response) const;

    Ui::SNSMessageDetailsDialog *_ui;
    QString _messageId;
    SNSService *_snsService;
    bool _changed = false;

    /**
     *  @brief Table data model
     */
    QStandardItemModel *_dataModel;
};

#endif // AWSMOCK_QT_UI_SNS_MESSAGE_DETAILS_DIALOG_H
