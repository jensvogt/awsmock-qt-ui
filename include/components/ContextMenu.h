//
// Created by vogje01 on 2/15/26.
//

#pragma once

// Qt includes
#include <QMenu>

class ContextMenu : public QMenu {
    Q_OBJECT

public:
    /**
     * @brief Constructor
     *
     * @param parent parent widget
     */
    explicit ContextMenu(QWidget *parent = nullptr) : QMenu(parent) {
        setToolTipsVisible(true);
        setStyleSheet(
            "QMenu::item:disabled { color: #888; background: none; }"
            "QMenu::item:selected:enabled { font-style: italic; }"
        );
    }

    /**
     * @brief Destructor
     */
    ~ContextMenu() override = default;
};
