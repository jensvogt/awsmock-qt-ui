//
// Created by vogje01 on 3/28/26.
//

#ifndef AWSMOCK_QT_UI_DYNAMODB_EXPORT_TABLE_DIALOG_H
#define AWSMOCK_QT_UI_DYNAMODB_EXPORT_TABLE_DIALOG_H

// QT includes
#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>

// Awsmock includes
#include <utils/Configuration.h>
#include <modules/dynamodb/DynamoDbService.h>

QT_BEGIN_NAMESPACE

namespace Ui {
    class DynamoDbExportTableDialog;
}

QT_END_NAMESPACE

class DynamoDbExportTableDialog : public QDialog {
    Q_OBJECT

public:
    /**
     * @brief Export the table items to a file.
     *
     * @param tableName name of the table
     * @param parent parent widget
     */
    explicit DynamoDbExportTableDialog(const QString &tableName, QWidget *parent = nullptr);

    /**
     * Destructor
     */
    ~DynamoDbExportTableDialog() override;

    void HandleAccept();

    void HandleReject();

    void LoadItemData() const;

    void HandleExportItems(const QString &exportItems) const;

private:
    /**
     * @brief Select an output file
     */
    void SelectFilename();

    /**
     *  @brief UI components
     */
    Ui::DynamoDbExportTableDialog *_ui;

    /**
     * @brief Output file
     */
    QString _file;

    /**
     * @brief Table name
     */
    QString _tableName;

    /**
     * @brief Module service
     */
    DynamoDbService *_dynamodbService;
};


#endif //AWSMOCK_QT_UI_DYNAMODB_EXPORT_TABLE_DIALOG_H
