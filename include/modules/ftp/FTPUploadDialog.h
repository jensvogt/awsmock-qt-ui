//
// Created by vogje01 on 11/8/25.
//

#ifndef AWSMOCK_QT_UI_FTP_UPLOAD_DIALOG_H
#define AWSMOCK_QT_UI_FTP_UPLOAD_DIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QValidator>
#include <QMessageBox>
#include <QDropEvent>
#include <QFileInfo>
#include <QMimeData>

#include <utils/NonEmptyValidator.h>
#include <modules/ftp/FTPClient.h>

QT_BEGIN_NAMESPACE

namespace Ui {
    class FTPUploadDialog;
}

QT_END_NAMESPACE

class FTPUploadDialog final : public QDialog {
    Q_OBJECT

public:
    explicit FTPUploadDialog(QWidget *parent = nullptr);

    ~FTPUploadDialog() override;

    void UpdateLineEditStyle(const QString &text) const;

    static void SetLineEditColor(QLineEdit *lineEdit, QValidator::State state);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;

    void dropEvent(QDropEvent *event) override;

private Q_SLOTS:
    void VerifyConnectInputs();

    void VerifyFileInputs();

private:
    void InitFtpClient();

    void HandleReject();

    void HandleAccept();

    Ui::FTPUploadDialog *ui;
    QFileInfo sourceFileInfo;
    embeddedmz::CFTPClient *ftpClient;
};


#endif // AWSMOCK_QT_UI_FTP_UPLOAD_DIALOG_H
