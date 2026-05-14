//
// Created by jensv on 05/05/2026.
//

#ifndef AWSMOCK_QT_UI_S3_MESSAGE_WINDOW_H
#define AWSMOCK_QT_UI_S3_MESSAGE_WINDOW_H

// C++ includes
#include <utility>

// Qt includes
#include <QDialog>
#include <QDialogButtonBox>

// Awsmock includes
#include <modules/s3/S3Service.h>
#include <modules/sqs/SQSService.h>
#include <utils/BaseDialog.h>
#include <utils/IconUtils.h>

QT_BEGIN_NAMESPACE

namespace Ui {
    class S3ObjectWindow;
}

QT_END_NAMESPACE

class S3ObjectWindow : public BaseDialog {
    Q_OBJECT

public:
    /**
     * @brief Constructor
     *
     * @param objectId SQS message ID
     * @param parent parent widget
     */
    explicit S3ObjectWindow(QString objectId, QWidget *parent = nullptr);

    /**
     * @brief Destructor
     */
    ~S3ObjectWindow() override;

    /**
     * @brief Load dialog content
     */
    void LoadContent() override;

    /**
     * @brief Update status bar
     */
    void SetLastUpdate() const;

    /**
     * @brief Update dialog content
     */
    void UpdateContent(const S3GetObjectDetailsResponse &response) const;

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
    Ui::S3ObjectWindow *_ui{};

    /**
     * @brief S3 service
     */
    S3Service *_s3Service{};

    /**
     * @brief S3 object ID
     */
    QString _objectId;
};


#endif //AWSMOCK_QT_UI_S3_MESSAGE_WINDOW_H
