//
// Created by vogje01 on 12/13/25.
//

#ifndef AWSMOCK_QT_UI_S3_OBJECT_ADD_DIALOG_H
#define AWSMOCK_QT_UI_S3_OBJECT_ADD_DIALOG_H

#include <QDialog>


QT_BEGIN_NAMESPACE

namespace Ui {
    class S3ObjectAddDialog;
}

QT_END_NAMESPACE

class S3ObjectAddDialog : public QDialog {
    Q_OBJECT

public:
    explicit S3ObjectAddDialog(QWidget *parent = nullptr);

    ~S3ObjectAddDialog() override;

    void BrowseSourceFile() const;

    void HandleAccept();

    void HandleReject();

    QString GetS3ObjectKey() {
        return _s3Key;
    }

    QString GetFilename() {
        return _fileName;
    }

private:
    Ui::S3ObjectAddDialog *_ui;

    /**
     * @brief Full qualified file name
     */
    QString _fileName;

    /**
     * @brief S3 object key
     */
    QString _s3Key;
};


#endif //AWSMOCK_QT_UI_S3_OBJECT_ADD_DIALOG_H
