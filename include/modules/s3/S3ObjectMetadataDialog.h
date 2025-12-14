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
    explicit S3ObjectMetadataDialog(QWidget *parent = nullptr, bool isAdded = true);

    ~S3ObjectMetadataDialog() override;

    void HandleAccept();

    void HandleReject();

    QString GetKey() const;

    QString GetValue() const;

private:
    Ui::S3ObjectMetadataDialog *_ui;
};


#endif //AWSMOCK_QT_UI_S3_OBJECT_METADATA_DIALOG_H
