//
// Created by vogje01 on 11/27/25.
//

#ifndef AWSMOCK_QT_UI_S3OBJECTMETADATADETAILSDIALOG_H
#define AWSMOCK_QT_UI_S3OBJECTMETADATADETAILSDIALOG_H

#include <QDialog>


QT_BEGIN_NAMESPACE

namespace Ui {
    class S3ObjectMetadataDetailsDialog;
}

QT_END_NAMESPACE

class S3ObjectMetadataDetailsDialog : public QDialog {
    Q_OBJECT

public:
    explicit S3ObjectMetadataDetailsDialog(QWidget *parent = nullptr);

    ~S3ObjectMetadataDetailsDialog() override;

    void HandleAccept();

    void HandleReject();

private:
    Ui::S3ObjectMetadataDetailsDialog *_ui;
};


#endif //AWSMOCK_QT_UI_S3OBJECTMETADATADETAILSDIALOG_H
