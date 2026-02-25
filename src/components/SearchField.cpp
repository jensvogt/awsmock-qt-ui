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
        _ui->clearButton->setIcon(IconUtils::GetIcon("clear"));
        _ui->searchFowardButton->setText(nullptr);
        _ui->searchFowardButton->setIcon(IconUtils::GetIcon("down"));
        _ui->searchBackwardButton->setText(nullptr);
        _ui->searchBackwardButton->setIcon(IconUtils::GetIcon("up"));

        // Text field
        connect(_ui->searchEdit, &QLineEdit::textChanged, this, [this](const QString &text) {
            _searchText = text;
            emit SigSearchForward(_searchText);
        });

        // Search buttons
        connect(_ui->searchFowardButton, &QPushButton::clicked, this, [this]() {
            emit SigSearchForward(_searchText);
        });
        connect(_ui->searchBackwardButton, &QPushButton::clicked, this, [this]() {
            emit SigSearchBackward(_searchText);
        });

        // Clear button
        connect(_ui->clearButton, &QPushButton::clicked, this, &SearchField::ClearSearchField);

        // Search type combo
        const QStringList searchTypes = {"contains", "startWith", "endsWith", "regexp"};
        _ui->searchTypeBox->addItems(searchTypes);
        connect(_ui->searchTypeBox, &QComboBox::currentTextChanged, this, [this](const QString &text) {
            _searchType = text;
        });

        // CLose label
        _ui->closeButton->setText(nullptr);
        _ui->closeButton->setIcon(IconUtils::GetIcon("close"));
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
}
