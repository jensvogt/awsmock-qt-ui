//
// Created by vogje01 on 2/15/26.
//

// You may need to build the project (run Qt uic code generator) to get "ui_PageableTable.h" resolved

#include <components/PageableTable.h>
#include "ui_PageableTable.h"
#include "utils/IconUtils.h"

PageableTable::PageableTable(QWidget *parent) : QWidget(parent), _ui(new Ui::PageableTable) {

    // Set defazkt page size
    _pageSize = Configuration::instance().GetValue<int>("ui.page-size");

    // Setup component
    _ui->setupUi(this);

    // Table
    _dataModel = new QStandardItemModel(this);
    _dataModel->setHorizontalHeaderLabels(_headerNames);
    _dataModel->setColumnCount(static_cast<int>(_headerNames.count()));

    // Proxy model for prefix filtering
    _proxyModel = new PrefixFilterProxyModel(this);
    _proxyModel->setSourceModel(_dataModel);

    // Table definition
    _ui->tableView->setModel(_proxyModel);
    _ui->tableView->setShowGrid(true);
    _ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    _ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    _ui->tableView->setSortingEnabled(true);
    _ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Start button
    _ui->startButton->setText(nullptr);
    _ui->startButton->setIcon(IconUtils::GetIcon("begin"));
    connect(_ui->startButton, &QPushButton::clicked, this, [this]() {
        _pageIndex = 0;
        CalculatePageStatus();
        emit pageChanged(_pageIndex, _pageSize);
    });

    _ui->previousButton->setText(nullptr);
    _ui->previousButton->setIcon(IconUtils::GetIcon("previous"));
    connect(_ui->previousButton, &QPushButton::clicked, this, [this]() {
        _pageIndex--;
        if (_pageIndex < 0) {
            _pageIndex = 0;
        }
        CalculatePageStatus();
        emit pageChanged(_pageIndex, _pageSize);
    });

    _ui->nextButton->setText(nullptr);
    _ui->nextButton->setIcon(IconUtils::GetIcon("next"));
    connect(_ui->nextButton, &QPushButton::clicked, this, [this]() {
        _pageIndex++;
        if (_pageIndex > _maxPage) {
            _pageIndex = _maxPage;
        }
        CalculatePageStatus();
        emit pageChanged(_pageIndex, _pageSize);
    });

    _ui->endButton->setText(nullptr);
    _ui->endButton->setIcon(IconUtils::GetIcon("end"));
    connect(_ui->endButton, &QPushButton::clicked, this, [this]() {
        _pageIndex = _maxPage;
        CalculatePageStatus();
        emit pageChanged(_pageIndex, _pageSize);
    });

    _ui->pageSizeEdit->setText(QString::number(_pageSize));
    connect(_ui->pageSizeEdit, &QLineEdit::textChanged, this, [this](const QString &text) {
        _pageSize = text.toLong();
        _maxPage = (_totalSize + _pageSize - 1) / _pageSize;
        CalculatePageStatus();
        emit pageChanged(_pageIndex, _pageSize);
    });

    // Defaults
    _ui->pageStatusLabel->setText(QString("%1 - %2/%3").arg(0).arg(_pageSize).arg(_totalSize));
}

PageableTable::~PageableTable() {
    delete _ui;
}

void PageableTable::CalculatePageStatus() const {
    long start = _pageIndex * _pageSize;
    if (start >= _totalSize) {
        start -= _pageSize;
    }
    long end = _pageIndex * _pageSize + _pageSize;
    if (end > _totalSize) {
        end = _totalSize;
    }
    _ui->pageStatusLabel->setText(QString("%1 - %2 / %3").arg(start).arg(end).arg(_totalSize));
}

void PageableTable::SetHeaderNames(const QStringList &headerNames) {
    _headerNames = headerNames;
    _dataModel->setHorizontalHeaderLabels(_headerNames);
    _dataModel->setColumnCount(static_cast<int>(_headerNames.count()));
}

void PageableTable::SetResizeModes(const QList<QHeaderView::ResizeMode> &resizeModes) const {
    for (int i = 0; i < static_cast<int>(resizeModes.count()); i++) {
        _ui->tableView->horizontalHeader()->setSectionResizeMode(i, resizeModes.at(i));
    }
}
