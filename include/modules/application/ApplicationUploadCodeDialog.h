//
// Created by vogje01 on 11/8/25.
//

#ifndef AWSMOCK_QT_UI_APPLICATION_UPLOAD_CODE_DIALOG_H
#define AWSMOCK_QT_UI_APPLICATION_UPLOAD_CODE_DIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE

namespace Ui {
    class ApplicationUploadCodeDialog;
}

QT_END_NAMESPACE

class ApplicationUploadCodeDialog final : public QDialog {
    Q_OBJECT

public:
    explicit ApplicationUploadCodeDialog(const QString &name, QWidget *parent = nullptr);

    ~ApplicationUploadCodeDialog() override;

    void HandleAccept();

    void HandleReject();

private:
    Ui::ApplicationUploadCodeDialog *ui;
};


#endif //AWSMOCK_QT_UI_APPLICATION_UPLOAD_CODE_DIALOG_H
