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
#include <QFileDialog>

#include <utils/Configuration.h>
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
    /**
     * @brief Constructor
     *
     * @param parent parent widget
     */
    explicit FTPUploadDialog(QWidget *parent = nullptr);

    /**
     * @brief Destructor
     */
    ~FTPUploadDialog() override;

    /**
     * @brief Text input verification
     *
     * @param text text to verify
     */
    void UpdateLineEditStyle(const QString &text) const;

    /**
     * @brief Sets the line edit color.
     *
     * @param lineEdit line edit widget
     * @param state validator status
     */
    static void SetLineEditColor(QLineEdit *lineEdit, QValidator::State state);

protected:
    /**
     * @brief Drag event callback.
     *
     * @param event drag event
     */
    void dragEnterEvent(QDragEnterEvent *event) override;

    /**
     * @brief Drop event callback
     *
     * @param event drop event
     */
    void dropEvent(QDropEvent *event) override;

private Q_SLOTS:
    /**
     * @brief Verification of the connect input fields
     */
    void VerifyConnectInputs();

    /**
     * @brief Verification of the file input fields
     */
    void VerifyFileInputs();

private:
    /**
     * @brief Initialize the FTP client
     */
    void InitFtpClient();

    /**
     * @brief Dialog reject callback
     */
    void HandleReject();

    /**
     * @brief Dialog accept callback
     */
    void HandleAccept();

    /**
     * @brief Source file browse button callback
     */
    void BrowseSourceFile();

    /**
     * UI components
     */
    Ui::FTPUploadDialog *ui;

    /**
     * @brief Source file infos
     */
    QFileInfo sourceFileInfo;

    /**
     * @brief FTP client using CURL FTP client
     */
    embeddedmz::CFTPClient *ftpClient;
};


#endif // AWSMOCK_QT_UI_FTP_UPLOAD_DIALOG_H
