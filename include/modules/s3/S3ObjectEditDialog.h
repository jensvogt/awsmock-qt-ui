//
// Created by vogje01 on 11/24/25.
//

#ifndef AWSMOCK_QT_UI_S3_OBJECT_EDIT_DIALOG_H
#define AWSMOCK_QT_UI_S3_OBJECT_EDIT_DIALOG_H

// Qt includes
#include <QEvent>
#include <QMenu>
#include <QPixmap>
#include <QPlainTextEdit>
#include <QScrollArea>
#include <QShortcut>
#include <QtGlobal>
#include <QWheelEvent>

// AwsMock includes
#include <modules/s3/S3ObjectMetadataDialog.h>
#include <modules/s3/S3ObjectWindow.h>
#include <modules/s3/S3Service.h>
#include <utils/BaseDialog.h>
#include <utils/DateTimeUtils.h>
#include <utils/IconUtils.h>
#include <utils/ImageUtils.h>
#include <utils/StringUtils.h>

QT_BEGIN_NAMESPACE

namespace Ui {
    class S3ObjectEditDialog;
}

QT_END_NAMESPACE

class QScrollArea;

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
     * @brief Show context menu
     *
     * @param pos mouse position
     */
    void ShowDefaultMetadataContextMenu(const QPoint &pos);

    /**
     * @brief Handle accept events
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
    void UpdateObject(const S3GetObjectDetailsResponse &objectDetailsResponse);

    /**
     * @brief Load dialog content
     */
    void LoadContent() override {
    }

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    void ApplyImageScale() const;

    void SetImageZoom(qreal zoom);

    void FitImageToViewport() const;

    /**
     * @brief Save the body to a file
     */
    void SaveToFile() const;

    /**
     * @biref Opens a save file dialog and returns the selected file path.
     *
     * @return file path
     */
    static QString SelectFilename();

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

    /**
     * @brief Plain text edit
     */
    QPlainTextEdit *_plaintextEdit;

    /**
     * @brief Image label
     */
    QLabel *_imageLabel;

    /**
     * @brief Image scroll area
     */
    QScrollArea *_imageScrollArea;

    /**
     * @brief Original image
     */
    QPixmap _imageOriginal;

    /**
     * @brief Current zoom factor
     */
    qreal _imageZoom = 1.0;

    /**
     * @brief Local copy of object response
     */
    S3GetObjectDetailsResponse _objectDetails;

    /**
     * @brief Fit-to-window toggle
     */
    bool _imageFitToWindow = false;

    /**
     * @brief Max size for body plain text edit
     */
    long _maxSize = 100 * 1024;
};


#endif // AWSMOCK_QT_UI_S3_OBJECT_EDIT_DIALOG_H
