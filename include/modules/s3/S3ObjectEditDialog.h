//
// Created by vogje01 on 11/24/25.
//

#ifndef AWSMOCK_QT_UI_S3OBJECTEDITDIALOG_H
#define AWSMOCK_QT_UI_S3OBJECTEDITDIALOG_H

#include <QDialog>

#include <modules/s3/S3Service.h>
#include <utils/IconUtils.h>
#include <utils/BaseDialog.h>

QT_BEGIN_NAMESPACE

namespace Ui {
    class S3ObjectEditDialog;
}

QT_END_NAMESPACE

class S3ObjectEditDialog final : public BaseDialog {
    Q_OBJECT

public:
    explicit S3ObjectEditDialog(const QString &objectId, QWidget *parent = nullptr);

    ~S3ObjectEditDialog() override;

    void HandleAccept();

    void HandleReject();

    void UpdateObject(const S3GetObjectDetailsResponse &objectDetailsResponse) const;

private:
    /**
     * @brief UI components
     */
    Ui::S3ObjectEditDialog *_ui;

    /**
     * @REST service handler
     */
    S3Service *_s3Service;

    /**
     * @brief Object ID
     */
    QString _objectId;

    /**
     * @brief Sort column index
     *
     * @par Default sort column is 'objects', index=1
     */
    int _metadataSortColumn = 1;

    /**
     * @brief Sort order
     */
    Qt::SortOrder _metadataSortOrder = Qt::DescendingOrder;
};


#endif //AWSMOCK_QT_UI_S3OBJECTEDITDIALOG_H
