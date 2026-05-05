//
// Created by jensv on 24/02/2026.
//

#include <components/SearchField.h>
#include "ui_SearchField.h"

namespace Awsmock::Components {

    SearchField::SearchField(QWidget *parent) : QWidget(parent), _ui(new Ui::SearchField) {
        // Setup UI components
        _ui->setupUi(this);

        // Buttons
        _ui->clearButton->setText(nullptr);
        _ui->clearButton->setToolTip("Clear text area");
        _ui->clearButton->setIcon(IconUtils::GetIcon("clear"));
        _ui->searchFowardButton->setText(nullptr);
        _ui->searchFowardButton->setToolTip("Search forward (F3)");
        _ui->searchFowardButton->setShortcut(Qt::Key_F3);
        _ui->searchFowardButton->setIcon(IconUtils::GetIcon("down"));
        _ui->searchBackwardButton->setText(nullptr);
        _ui->searchBackwardButton->setToolTip("Search backward (Shift + F3)");
        _ui->searchBackwardButton->setShortcut(Qt::Key_F3 | Qt::SHIFT);
        _ui->searchBackwardButton->setIcon(IconUtils::GetIcon("up"));

        // Text field
        connect(_ui->searchEdit, &QLineEdit::textChanged, this, [this](const QString &text) {
            _searchText = text;
            emit SigSearchForward(_searchText, SearchTypeFromString(_searchType));
        });

        // Search buttons
        connect(_ui->searchFowardButton, &QPushButton::clicked, this, [this]() {
            emit SigSearchForward(_searchText, SearchTypeFromString(_searchType));
        });
        connect(_ui->searchBackwardButton, &QPushButton::clicked, this, [this]() {
            emit SigSearchBackward(_searchText, SearchTypeFromString(_searchType));
        });

        // Clear button
        connect(_ui->clearButton, &QPushButton::clicked, this, &SearchField::ClearSearchField);

        // Search type combo
        const QStringList searchTypes = GetSearchTypeNames();
        _ui->searchTypeBox->addItems(searchTypes);
        _ui->searchTypeBox->setToolTip("Search type");
        connect(_ui->searchTypeBox, &QComboBox::currentTextChanged, this, [this](const QString &text) {
            _searchType = text;
            emit SigSearchTypeChanged(SearchTypeFromString(_searchType));
        });

        // CLose label
        _ui->closeButton->setText(nullptr);
        _ui->closeButton->setToolTip("Close the search widget");
        _ui->closeButton->setIcon(IconUtils::GetIcon("clear"));
        connect(_ui->closeButton, &QPushButton::clicked, this, [this]() {
            emit SigClose();
        });

    }

    SearchField::~SearchField() {
        delete _ui;
    }

    void SearchField::ClearSearchField() {

        _ui->searchEdit->setText(nullptr);
        _searchText = nullptr;
    }

    // In your dialog's showEvent override:
    void SearchField::showEvent(QShowEvent *event) {
        _ui->searchEdit->setFocus();
    }
}
