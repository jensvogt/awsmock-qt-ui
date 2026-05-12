//
// Created by vogje01 on 5/12/26.
//

// You may need to build the project (run Qt uic code generator) to get "ui_ListFilterWidget.h" resolved

#include <components/ListFilterWidget.h>
#include "ui_ListFilterWidget.h"

namespace Awsmock::Components {
    ListFilterWidget::ListFilterWidget(QWidget *parent) : QWidget(parent), _ui(new Ui::ListFilterWidget) {

        // Setup UI
        _ui->setupUi(this);

        // Connect server filter combo
        const QStringList serverFilterTypes = {"Contains", "Prefix", "Postfix", "Regular Expression"};
        _ui->filterTypeCombo->addItems(FilterTypesToList());
        _ui->filterTypeCombo->setCurrentText(FilterTypeToString(_currentFilter));
        connect(_ui->filterTypeCombo, &QComboBox::currentTextChanged, this, [this](const QString &value) {
            _currentFilter = FilterTypeFromString(value);
            ApplyFilter(_ui->filterEdit->text());
        });

        // Filter clear button
        _ui->clearButton->setText(nullptr);
        _ui->clearButton->setIcon(IconUtils::GetIcon("clear"));
        connect(_ui->clearButton, &QPushButton::clicked, this, &ListFilterWidget::ClearFilter);

        // Connect server filter changes
        connect(_ui->filterEdit, &QLineEdit::textChanged, this, [this](const QString &text) {
            ApplyFilter(text);
        });
    }

    ListFilterWidget::~ListFilterWidget() {
        delete _ui;
    }

    void ListFilterWidget::ApplyFilter(const QString &text) const {
        if (!_proxyModel) {
            return;
        }
        switch (_ui->filterTypeCombo->currentIndex()) {
            case LIST_FILTER_CONTAINS:
                _proxyModel->setFilterFixedString(text);
                break;
            case LIST_FILTER_PREFIX:
                _proxyModel->setFilterRegularExpression("^" + QRegularExpression::escape(text));
                break;
            case LIST_FILTER_POSTFIX:
                _proxyModel->setFilterRegularExpression(QRegularExpression::escape(text) + "$");
                break;
            case LIST_FILTER_REGULAR_EXPRESSION:
                if (const QRegularExpression rx(text); rx.isValid())
                    _proxyModel->setFilterRegularExpression(rx);
                break;
            default: ;
        }
    }

    void ListFilterWidget::ClearFilter() {
        _ui->filterEdit->setText(nullptr);
        _ui->filterTypeCombo->setCurrentIndex(0);
        if (_proxyModel) {
            _proxyModel->setFilterRegularExpression(QRegularExpression());
        }

        emit FilterCleared();
    }
} // Awsmock::Components
