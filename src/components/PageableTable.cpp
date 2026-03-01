//
// Created by vogje01 on 2/15/26.
//

// You may need to build the project (run Qt uic code generator) to get "ui_PageableTable.h" resolved

#include <components/PageableTable.h>
#include "ui_PageableTable.h"
#include "modules/cognito/CognitoUserpoolList.h"

PageableTable::PageableTable(QWidget *parent) : QWidget(parent), _ui(new Ui::PageableTable) {

    // Set defaaut page size
    _pageSize = Configuration::instance().GetValue<int>("ui.page-size");

    // Setup component
    _ui->setupUi(this);

    // Prefix edit
    _ui->prefixEdit->setPlaceholderText("Prefix");
    _ui->prefixEdit->setEnabled(true);
    connect(_ui->prefixEdit, &QLineEdit::textChanged, this, [this]() {
        _ui->prefixClearButton->setDisabled(false);
        _proxyModel->setFilterColumn(0);
        _proxyModel->setFilterPrefix(_ui->prefixEdit->text());
    });

    // Prefix clear button
    _ui->prefixClearButton->setDisabled(true);
    _ui->prefixClearButton->setText(nullptr);
    _ui->prefixClearButton->setIcon(IconUtils::GetIcon("clear"));
    _ui->prefixClearButton->setToolTip("Clear the prefix field");
    connect(_ui->prefixClearButton, &QPushButton::clicked, this, [this]() {
        _proxyModel->clearFilter();
        _ui->prefixEdit->setText(nullptr);
        _ui->prefixClearButton->setDisabled(true);
    });

    // Table
    _dataModel = new QStandardItemModel(this);
    _dataModel->setHorizontalHeaderLabels(_headerNames);
    _dataModel->setColumnCount(static_cast<int>(_headerNames.count()));

    // Proxy model for prefix filtering
    _proxyModel = new PrefixFilterProxyModel(this);
    _proxyModel->setSourceModel(_dataModel);
    _proxyModel->setDynamicSortFilter(true);

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
    });

    _ui->previousButton->setText(nullptr);
    _ui->previousButton->setIcon(IconUtils::GetIcon("previous"));
    connect(_ui->previousButton, &QPushButton::clicked, this, [this]() {
        _pageIndex--;
        if (_pageIndex < 0) {
            _pageIndex = 0;
        }
        CalculatePageStatus();
        emit ReloadTable();
    });

    _ui->nextButton->setText(nullptr);
    _ui->nextButton->setIcon(IconUtils::GetIcon("next"));
    connect(_ui->nextButton, &QPushButton::clicked, this, [this]() {
        _pageIndex++;
        if (_pageIndex >= _maxPage) {
            _pageIndex = _maxPage;
        }
        CalculatePageStatus();
        emit ReloadTable();
    });

    _ui->endButton->setText(nullptr);
    _ui->endButton->setIcon(IconUtils::GetIcon("end"));
    connect(_ui->endButton, &QPushButton::clicked, this, [this]() {
        _pageIndex = _maxPage;
        CalculatePageStatus();
        emit ReloadTable();
    });

    _ui->pageSizeEdit->setText(QString::number(_pageSize));
    connect(_ui->pageSizeEdit, &QLineEdit::textChanged, this, [this](const QString &text) {
        _pageSize = text.toLong();
        _maxPage = (_totalSize + _pageSize - 1) / _pageSize;
        CalculatePageStatus();
        emit ReloadTable();
    });

    // Add context menu
    _ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(_ui->tableView, &QTableView::customContextMenuRequested, this, [this](const QPoint pos) {
        emit ContextMenuRequested(pos);
    });

    // Single click proxy
    connect(_ui->tableView, &QTableView::clicked, this, [this](const QModelIndex &index) {
        emit SingleClick(index);
    });

    // Double click proxy
    connect(_ui->tableView, &QTableView::doubleClicked, this, [this](const QModelIndex &index) {
        emit DoubleClicked(index);
    });

    // Defaults
    _ui->pageStatusLabel->setText(QString("%1 - %2/%3").arg(0).arg(_pageSize).arg(_totalSize));
    SetLastUpdate();
}

PageableTable::~PageableTable() {
    delete _ui;
}

void PageableTable::CalculatePageStatus() {
    long start = _pageIndex * _pageSize;
    if (start >= _totalSize) {
        _pageIndex = --_pageIndex > 0 ? _pageIndex : 0;
        start = _pageIndex * _pageSize;
    }
    long end = _pageIndex * _pageSize + _pageSize;
    if (end > _totalSize) {
        end = _totalSize;
    }
    _ui->pageStatusLabel->setText(QString("%1 - %2 / %3").arg(start).arg(end).arg(_totalSize));
    SetLastUpdate();
}

void PageableTable::UpdateSorting() const {
    _proxyModel->sort(_sortColumn, _sortDirection == 1 ? Qt::AscendingOrder : Qt::DescendingOrder);
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

void PageableTable::SetHiddenColumns(const QList<int> &hiddenColumns) const {
    for (int i = 0; i < static_cast<int>(hiddenColumns.count()); i++) {
        _ui->tableView->setColumnHidden(hiddenColumns.at(i), true);
    }
}

void PageableTable::SetColumn(const int row, const int column, const QString &value, const Qt::Alignment &alignment) const {
    const auto item = new QStandardItem(value);
    item->setTextAlignment(alignment);
    const QModelIndex index = _dataModel->index(row, column);
    _dataModel->setData(index, QVariant(alignment));
    _dataModel->setData(index, value, Qt::EditRole);
    _dataModel->setData(index, value, Qt::DisplayRole);
    _dataModel->setItem(row, column, item);
}

void PageableTable::SetColumn(const int row, const int column, const QDateTime &value) const {
    _dataModel->setItem(row, column, new QStandardItem(DateTimeUtils::GetDateTimeFormat(value)));
}

void PageableTable::SetColumn(const int row, const int column, const long &value) const {
    const QModelIndex index = _dataModel->index(row, column);
    _dataModel->setData(index, QVariant(Qt::AlignRight | Qt::AlignVCenter), Qt::TextAlignmentRole);
    _dataModel->setData(index, static_cast<qlonglong>(value), Qt::UserRole);
    _dataModel->setData(index, static_cast<qlonglong>(value), Qt::DisplayRole);
}

void PageableTable::SetColumn(const int row, const int col, const bool value, const QIcon &enabledIcon, const QIcon &disabledIcon) const {
    auto *iconItem = new QStandardItem();
    iconItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    iconItem->setData(Qt::DisplayRole, value ? 1 : 0);
    iconItem->setText("");
    iconItem->setIcon(value ? enabledIcon : disabledIcon);
    _dataModel->setItem(row, col, iconItem);
}

void PageableTable::SetHiddenColumn(const int row, const int col, const QString &value) const {
    const auto item = new QStandardItem(value);
    item->setData(value, Qt::EditRole);
    _dataModel->setItem(row, col, item);
}

void PageableTable::SetHiddenColumn(const int row, const int col, const bool value) const {
    const auto checkItem = new QStandardItem();
    checkItem->setCheckState(value ? Qt::Checked : Qt::Unchecked);
    checkItem->setFlags(checkItem->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
    _dataModel->setItem(row, col, checkItem);
}

void PageableTable::SetStatus(const QString &message) const {
    _ui->statusLabel->setText(message);
}

void PageableTable::SetLastUpdate() const {
    const QString message = "Last update: " + DateTimeUtils::GetLogTimeFormat(QDateTime::currentDateTime());
    _ui->statusLabel->setText(message);
}

QModelIndex PageableTable::GetIndexFromPosition(const QPoint &pos) const {

    const QModelIndex proxyIndex = _ui->tableView->indexAt(pos);
    if (!proxyIndex.isValid())
        return {};

    return _proxyModel->mapToSource(proxyIndex);
}

QPoint PageableTable::GetGlobalPosition(const QPoint &tablePosition) const {
    return _ui->tableView->viewport()->mapToGlobal(tablePosition);
}

void PageableTable::RemoveRow(const QModelIndex &index) const {
    _dataModel->removeRow(index.row(), index.parent());
}

QModelIndex PageableTable::GetSourceIndex(const QModelIndex &index) const {
    return _proxyModel->mapToSource(index);
}

QModelIndexList PageableTable::GetSelectedRows() const {
    return _ui->tableView->selectionModel()->selectedRows();
}

void PageableTable::SetMultiRowSelection(const bool enabled) const {
    if (!enabled) {
        _ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    } else {
        _ui->tableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    }
}
