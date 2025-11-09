//
// Created by vogje01 on 11/8/25.
//

#ifndef AWSMOCK_QT_UI_APPLICATION_UPLOAD_CODE_DIALOG_H
#define AWSMOCK_QT_UI_APPLICATION_UPLOAD_CODE_DIALOG_H

#include <QDialog>
#include <QDropEvent>
#include <QFileInfo>
#include <QMimeData>
#include <QFileDialog>

#include <utils/FileUtils.h>
#include <modules/application/ApplicationService.h>

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

    void dragEnterEvent(QDragEnterEvent *event) override;

    void dropEvent(QDropEvent *event) override;

    void HandleBrowse();

    void HandleAccept();

    void HandleReject();

private:
    /**
     * UI connection
     */
    Ui::ApplicationUploadCodeDialog *ui;

    /**
    * @brief Service
    */
    ApplicationService *_applicationService;

    /**
     * @brief File information about the application code file to upload
     */
    QFileInfo _fileInfo;
};


#endif //AWSMOCK_QT_UI_APPLICATION_UPLOAD_CODE_DIALOG_H
