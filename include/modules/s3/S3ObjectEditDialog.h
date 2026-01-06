//
// Created by vogje01 on 11/24/25.
//

#ifndef AWSMOCK_QT_UI_S3_OBJECT_EDIT_DIALOG_H
#define AWSMOCK_QT_UI_S3_OBJECT_EDIT_DIALOG_H

// AwsMock includes
#include <modules/s3/S3Service.h>
#include <utils/IconUtils.h>
#include <utils/BaseDialog.h>
#include <utils/DateTimeUtils.h>

QT_BEGIN_NAMESPACE

namespace Ui {
    class S3ObjectEditDialog;
}

QT_END_NAMESPACE

class S3ObjectEditDialog final : public BaseDialog {
    Q_OBJECT

public:
    /**
     * @brief Constructor
     *
     * @param objectId S3 object ID
     * @param parent parent widget
     */
    explicit S3ObjectEditDialog(const QString &objectId, QWidget *parent = nullptr);

    /**
     * @brief Destructor
     */
    ~S3ObjectEditDialog() override;

    /**
     * @brief Handle accept ebemts
     */
    void HandleAccept();

    /**
     * @brief Handle cancel event
     */
    void HandleReject();

    /**
     * @brief Handle update object response
     *
     * @param objectDetailsResponse object details REST response
     */
    void UpdateObject(const S3GetObjectDetailsResponse &objectDetailsResponse) const;

    /**
     * @brief Load dialog content
     */
    void LoadContent() override {
    }

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

    /**
     * @brief Changed flag
     */
    bool _changed = false;
};


#endif // AWSMOCK_QT_UI_S3_OBJECT_EDIT_DIALOG_H
