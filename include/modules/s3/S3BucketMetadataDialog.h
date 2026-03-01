//
// Created by vogje01 on 11/27/25.
//

#ifndef AWSMOCK_QT_UI_S3_BUCKET_METADATA_DIALOG_H
#define AWSMOCK_QT_UI_S3_BUCKET_METADATA_DIALOG_H

#include <QDialog>
#include <QDialogButtonBox>

QT_BEGIN_NAMESPACE

namespace Ui {
    class S3BucketMetadataDialog;
}

QT_END_NAMESPACE

class S3BucketMetadataDialog : public QDialog {
    Q_OBJECT

public:
    /**
     * @brief Constructor for a new metadata dialog
     *
     * @param parent parent widget
     */
    explicit S3BucketMetadataDialog(QWidget *parent);

    /**
     * @brief Constructor for a edit metadata dialog.
     *
     * @param parent parent widget
     * @param name name of the metadata
     * @param value value of the metadata
     */
    S3BucketMetadataDialog(QWidget *parent, const QString &name, const QString &value);

    /**
     * @brief Destructor
     */
    ~S3BucketMetadataDialog() override;

    /**
     * @brief Initialize common UI components
     */
    void Initialize();

    /**
     * @brief Handle 'OK' button events
     */
    void HandleAccept();

    /**
     * @brief Handle 'CANCEL' button events
     */
    void HandleReject();

    /**
     * @brief Get the key string
     *
     * @return key as string
     */
    [[nodiscard]] QString GetKey() const;

    /**
     * @brief Get the value string
     *
     * @return value string
     */
    [[nodiscard]] QString GetValue() const;

private:
    /**
     * @brief UI components
     */
    Ui::S3BucketMetadataDialog *_ui;
};


#endif // AWSMOCK_QT_UI_S3_BUCKET_METADATA_DIALOG_H
