//
// Created by vogje01 on 11/27/25.
//

#ifndef AWSMOCK_QT_UI_S3_OBJECT_METADATA_DIALOG_H
#define AWSMOCK_QT_UI_S3_OBJECT_METADATA_DIALOG_H

#include <QDialog>
#include <QDialogButtonBox>

QT_BEGIN_NAMESPACE

namespace Ui {
    class S3ObjectMetadataDialog;
}

QT_END_NAMESPACE

class S3ObjectMetadataDialog : public QDialog {
    Q_OBJECT

public:
    /**
     * @brief Constructor for new metadata
     *
     * @param parent parent widget
     */
    explicit S3ObjectMetadataDialog(QWidget *parent = nullptr);

    explicit S3ObjectMetadataDialog(const QString &key, const QString &value, QWidget *parent = nullptr);

    ~S3ObjectMetadataDialog() override;

    void Initialize();

    void HandleAccept();

    void HandleReject();

    [[nodiscard]] QString GetKey() const;

    [[nodiscard]] QString GetValue() const;

private:
    Ui::S3ObjectMetadataDialog *_ui;
};


#endif //AWSMOCK_QT_UI_S3_OBJECT_METADATA_DIALOG_H
