//
// Created by vogje01 on 11/9/25.
//

#pragma once

// Qt includes
#include <QStandardItemModel>
#include <QTableWidget>

// Awsmock includes
#include <utils/DateTimeUtils.h>

class TableUtils {
public:
    TableUtils() = default;

    ~TableUtils() = default;

    static void SetColumn(QTableWidget *tableWidget, int row, int col, const QString &value);

    static void SetColumn(QTableWidget *tableWidget, int row, int col, int value);

    static void SetColumn(QTableWidget *tableWidget, int row, int col, long value);

    static void SetColumn(QTableWidget *tableWidget, int row, int col, double value, int digits = 3);

    static void SetColumn(QTableWidget *tableWidget, int row, int col, const QDateTime &value);

    static void SetColumn(QTableWidget *tableWidget, int row, int col, bool value, const QIcon &enabledIcon, const QIcon &disabledIcon);

    static void SetHiddenColumn(QTableWidget *tableWidget, int row, int col, const QString &value);

    static void SetHiddenColumn(QStandardItemModel *tableModel, int row, int col, const QString &value);

    static void SetHiddenColumn(QTableWidget *tableWidget, int row, int col, bool value);

    static void SetColumn(QStandardItemModel *dataModel, int row, int col, const QString &value, Qt::Alignment alignment = Qt::AlignLeft | Qt::AlignVCenter);

    static void SetColumn(QStandardItemModel *dataModel, int row, int col, int value);

    static void SetColumn(QStandardItemModel *dataModel, int row, int col, long value);

    static void SetColumn(QStandardItemModel *dataModel, int row, int col, double value, int digits = 3);

    static void SetColumn(QStandardItemModel *dataModel, int row, int col, const QDateTime &value);

    static void SetColumn(QStandardItemModel *dataModel, int row, int col, bool value, const QIcon &enabledIcon, const QIcon &disabledIcon);
};
