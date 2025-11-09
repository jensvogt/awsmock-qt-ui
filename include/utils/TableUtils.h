//
// Created by vogje01 on 11/9/25.
//

#ifndef AWSMOCK_QT_UI_TABLE_UTILS_H
#define AWSMOCK_QT_UI_TABLE_UTILS_H

#include <QTableWidget>
#include <QDateTime>

class TableUtils {
public:
    TableUtils() {
    };

    ~TableUtils() = default;

    static void SetColumn(QTableWidget *tableWidget, int row, int col, const QString &value);

    static void SetColumn(QTableWidget *tableWidget, int row, int col, int value);

    static void SetColumn(QTableWidget *tableWidget, int row, int col, long value);

    static void SetColumn(QTableWidget *tableWidget, int row, int col, const QDateTime &value);

    static void SetColumn(QTableWidget *tableWidget, int row, int col, bool value, const QIcon &enabledIcon, const QIcon &disabledIcon);

    static void SetHiddenColumn(QTableWidget *tableWidget, int row, int col, const QString &value);

    static void SetHiddenColumn(QTableWidget *tableWidget, int row, int col, bool value);
};
#endif //AWSMOCK_QT_UI_TABLE_UTILS_H
