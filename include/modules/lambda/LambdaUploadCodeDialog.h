//
// Created by vogje01 on 11/8/25.
//

#ifndef AWSMOCK_QT_UI_LAMBDA_UPLOAD_CODE_DIALOG_H
#define AWSMOCK_QT_UI_LAMBDA_UPLOAD_CODE_DIALOG_H

#include <QDialog>
#include <QDropEvent>
#include <QFileInfo>
#include <QMimeData>
#include <QFileDialog>

#include <utils/FileUtils.h>
#include <modules/lambda/LambdaService.h>
#include <dto/lambda/LambdaUploadRequest.h>

QT_BEGIN_NAMESPACE

namespace Ui {
    class LambdaUploadCodeDialog;
}

QT_END_NAMESPACE

class LambdaUploadCodeDialog final : public QDialog {
    Q_OBJECT

public:
    explicit LambdaUploadCodeDialog(const QString &lambdaName, const QString &lambdaArn, QWidget *parent = nullptr);

    ~LambdaUploadCodeDialog() override;

    void dragEnterEvent(QDragEnterEvent *event) override;

    void dropEvent(QDropEvent *event) override;

    void HandleBrowse();

    void HandleAccept();

    void HandleReject();

private:
    /**
     * UI connection
     */
    Ui::LambdaUploadCodeDialog *ui;

    /**
    * @brief Service
    */
    LambdaService *_lambdaService;

    /**
     * @brief File information about the lambda code file to upload
     */
    QFileInfo _fileInfo;

    /**
     * @brief Name of the lambda function
     */
    QString _lambdaName;

    /**
     * @brief AWS ARN of the lambda function
     */
    QString _lambdaArn;
};


#endif //AWSMOCK_QT_UI_LAMBDA_UPLOAD_CODE_DIALOG_H
