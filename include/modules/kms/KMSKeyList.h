#ifndef AWSMOCK_QT_UI_KMS_KEY_LIST_H
#define AWSMOCK_QT_UI_KMS_KEY_LIST_H

// Qt includes
#include <QLabel>
#include <QMenu>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QListWidget>
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QLineEdit>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QToolBar>
#include <QPushButton>
#include <QInputDialog>
#include <QFormLayout>
#include <QTreeWidget>
#include <QTimer>

// Awsmock includes
#include <utils/BasePage.h>
#include <utils/IconUtils.h>
#include <utils/DateTimeUtils.h>
#include <utils/PrefixFilterModel.h>
#include <dto/kms/KMSListKeysRequest.h>
#include <dto/kms/KMSListKeysResponse.h>
#include <modules/kms/KMSService.h>
#include <modules/kms/KMSKeyDialog.h>
#include <components/PageableTable.h>

class KMSKeyList final : public BasePage {
    Q_OBJECT

public:
    /**
     * @brief KMS key list
     *
     * @param title widget title
     * @param parent parent widget
     */
    explicit KMSKeyList(const QString &title, QWidget *parent = nullptr);

    /**
     * Destructor
     */
    ~KMSKeyList() override;

    /**
     * @brief Clear the page content
     */
    void ClearContent() override {
    }

    /**
     * @brief Load page content
     */
    void LoadContent() override;

    /**
     * @brief Load page content
     *
     * @param listKeysResponse list KMS keys response
     */
    void HandleListKeysSignal(const KMSListKeysResponse &listKeysResponse) const;

signals:
    /**
     * @brief List keys signal
     */
    void ListKeysSignal();

private slots:
    /**
     * @brief Context menu callback
     */
    void ShowContextMenu(const QPoint &pos);

private:
    /**
     * @brief AWS region
     */
    QString _region;

    /**
     * @brief Table view
     */
    PageableTable *_tableView;

    /**
     * @brief REST service handler
     */
    KMSService *_kmsService;
};

#endif // AWSMOCK_QT_UI_KMS_KEY_LIST_H
