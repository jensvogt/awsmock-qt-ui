//
// Created by vogje01 on 11/8/25.
//

#pragma once

#include <QDialog>
#include <QDropEvent>
#include <QFileInfo>
#include <QMimeData>
#include <QFileDialog>

#include <utils/FileUtils.h>
#include <utils/IconUtils.h>
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
    /**
     * @brief Constructor
     *
     * @param lambdaName name of the lambda
     * @param lambdaArn lambda ARN
     * @param parent parent widget
     */
    explicit LambdaUploadCodeDialog(const QString &lambdaName, const QString &lambdaArn, QWidget *parent = nullptr);

    /**
     * @brief destructor
     */
    ~LambdaUploadCodeDialog() override;

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;

    void dropEvent(QDropEvent *event) override;

private:
    /**
     * @brief Handle browse button events
     */
    void HandleBrowse();

    /**
     * @brie Handle OK button events
     */
    void HandleAccept();

    /**
     * @brie Handle CANCEL button events
     */
    void HandleReject();

    /**
     * UI connection
     */
    Ui::LambdaUploadCodeDialog *_ui;

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
