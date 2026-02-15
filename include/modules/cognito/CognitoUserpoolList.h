//
// Created by vogje01 on 2/15/26.
//

#ifndef AWSMOCK_QT_UI_COGNITO_USERPOOL_LIST_H
#define AWSMOCK_QT_UI_COGNITO_USERPOOL_LIST_H

// Qt includes
#include <QObject>
#include <QLabel>
#include <QVBoxLayout>
#include <QMenu>
#include <QPushButton>

// Awsmock includes
#include <utils/BasePage.h>
#include <utils/IconUtils.h>
#include <components/PageableTable.h>
#include <modules/cognito/CognitoService.h>
#include <modules/cognito/CognitoAddUserpoolDialog.h>

/**
 * @brief Application list widget. The widget will be placed in the content pane.
 */
class CognitoUserpoolList : public BasePage {
    Q_OBJECT

public:
    /**
     * @brief Cognito user pool list
     *
     * @param title widget title
     * @param parent parent widget
     */
    explicit CognitoUserpoolList(const QString &title, QWidget *parent = nullptr);

    /**
     * Destructor
     */
    ~CognitoUserpoolList() override;

    /**
     * @brief Load content
     */
    void LoadContent() override;

    void HandleUserpoolList(const CognitoUserpoolListResponse &response) const;

    void ShowContextMenu(const QPoint &pos) const;

private:
    /**
     * @brief Layout
     */
    QVBoxLayout *_layout{};

    /**
     * @brief table
     */
    PageableTable *_table{};

    /**
     * @brief REST service
     */
    CognitoService *_cognitoService{};
};

#endif //AWSMOCK_QT_UI_COGNITO_USERPOOL_LIST_H
