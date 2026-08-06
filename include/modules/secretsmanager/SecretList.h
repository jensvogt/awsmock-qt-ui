//
// Created by vogje01 on 11/7/25.
//

#ifndef AWSMOCK_QT_UI_SECRET_LIST_H
#define AWSMOCK_QT_UI_SECRET_LIST_H

// QT includes
#include <QMenu>
#include <QLabel>
#include <QTableView>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QPushButton>
#include <QHeaderView>

// Awsmock includes
#include <utils/BasePage.h>
#include <utils/IconUtils.h>
#include <utils/PrefixFilterModel.h>
#include <components/PageableTable.h>
#include <modules/secretsmanager/SecretsManagerService.h>
#include <modules/secretsmanager/SecretsDetailsDialog.h>
#include <modules/secretsmanager/SecretAddDialog.h>

/**
 * @brief Secrets list widget. The widget will be placed in the content pane.
 */
class SecretList final : public BasePage {
    Q_OBJECT

public:
    /**
     * @brief Secretsmanager secret list
     *
     * @param title widget title
     * @param parent parent widget
     */
    explicit SecretList(const QString &title, QWidget *parent = nullptr);

    /**
     * Destructor
     */
    ~SecretList() override;

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
     * @param secretsListResponse secrets counter list
     */
    void HandleListSecretsSignal(const SecretsListResponse &secretsListResponse) const;

private slots:
    /**
     * @brief Show the context menu
     *
     * @param pos table position
     */
    void ShowContextMenu(const QPoint &pos);

    /**
     * @brief Delete the currently selected secret(s)
     */
    void DeleteSelected();

private:
    /**
     * @brief Service
     */
    SecretsManagerService *_secretsManagerService;

    /**
     * @brief Secrets table view
     */
    PageableTable *_tableView;
};

#endif // AWSMOCK_QT_UI_SECRET_LIST_H
