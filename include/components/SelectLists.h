//
// Created by vogje01 on 4/3/26.
//

#pragma once

// QT includes
#include <QButtonGroup>
#include <QGroupBox>
#include <QStandardItemModel>
#include <QWidget>

// Awsmock includes
#include <utils/IconUtils.h>

QT_BEGIN_NAMESPACE
namespace Ui {
    class SelectLists;
}

QT_END_NAMESPACE

namespace Awsmock::Components {

    using UiSelectLists = ::Ui::SelectLists;

    class SelectLists : public QWidget {
        Q_OBJECT

    public:
        /**
         * @brief Constructor
         *
         * @param parent parent widget
         */
        explicit SelectLists(QWidget *parent = nullptr);

        /**
         * @brief Destructor
         */
        ~SelectLists() override;

        /**
         * @brief Sets the available items
         *
         * @param available available string list
         */
        void SetAvailable(const QStringList &available);

        /**
         * @brief Sets the selected items
         *
         * @par
         * Selected items in the left data model will be removed.
         *
         * @param selected selected string list
         */
        void SetSelected(const QStringList &selected);

        /**
         * @brief Returns the list of available items
         *
         * @return string ist of selected items
         */
        QStringList GetSelected();

    signals:
        /**
         * @brief Signaled when the module selection changed
         *
         * @param selected string list of selected items
         */
        void SelectChangedSignal(const QStringList &selected);

    private:
        /**
         * @brief UI components
         */
        UiSelectLists *_ui;

        /**
         * @brief Initialize the button row
         */
        void SetupListButtons();

        /**
         * @brief Available items
         */
        QStringList _available;

        /**
         * @brief Selected items
         */
        QStringList _selected;

        /**
         * @brief Data model
         */
        QStandardItemModel *_leftDataModel;

        /**
         * @brief Data model
         */
        QStandardItemModel *_rightDataModel;
    };

} // Awsmock::Components

