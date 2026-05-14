//
// Created by vogje01 on 2/6/26.
//

#ifndef AWSMOCK_QT_UI_MAIN_WIDGET_H
#define AWSMOCK_QT_UI_MAIN_WIDGET_H

// Qt includes
#include <QWidget>
#include <QStringListModel>
#include <QStandardItemModel>
#include <QtWebSockets/QWebSocket>
#include <QStackedWidget>
#include <QAbstractItemView>

// Awsmock includes
#include <MainRouter.h>
#include <utils/IconUtils.h>
#include <utils/PrefixFilterModel.h>
#include <utils/ServerLogWidget.h>
#include <modules/module/ModuleService.h>

class QStandardItemModel;
QT_BEGIN_NAMESPACE

namespace Ui {
    class MainWidget;
}

QT_END_NAMESPACE

class MainWidget : public QWidget {
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = nullptr);

    ~MainWidget() override;

    void SetupNavPane();

    void SetupContentPane();

    void SetupLogPane();

    void SetupServerLogs();

    void SetupLocalLogs();

    void OnConnected() const;

    void OnMessageReceived(const QString &message) const;

    void SetupStatusbar();

private:
    /**
     * @brief UI Components
     */
    Ui::MainWidget *_ui;

    /**
     * @brief Websocket for the server logs
     */
    QWebSocket *_webSocket{};

    /**
     *  @brief Navigation item data model
     */
    QStandardItemModel *_navDataModel{};

    /**
     * @brief Server proxy model
     */
    QSortFilterProxyModel *_serverProxyModel{};

    /**
     *  @brief Log item data model
     */
    QStandardItemModel *_serverLogDataModel{};

    /**
     *  @brief Local log item data model
     */
    QStandardItemModel *_localLogDataModel{};

    /**
     * @brief Local proxy model
     */
    QSortFilterProxyModel *_localProxyModel{};
    
    /**
     * @brief Content pane
     */
    QStackedWidget *_contentPane{};

    /**
     * @brief Navigation list
     */
    QListView *_navList{};

    /**
     * @brief Server scrolling flag
     */
    bool _serverScrolling = true;

    /**
     * @brief Local scrolling flag
     */
    bool _localScrolling = true;

    /**
     * @brief External log dialog
     */
    ServerLogWidget *_externalLogDialog{};

    /**
     * @brief Websocket URL
     */
    QString _websocketUrl;

    /**
     * @brief Reconnect timer
     */
    QTimer *_reconnectTimer{};

    /**
     * @brief Local scrolling flag
     */
    LogLevel _localLogLevel = INFO;

    /**
     * @brief Log lines limit
     */
    long _logLimit = 500;

    /**
     * @brief Current log level, we start with 'warning'.
     */
    QString _currentLogLevel = "warning";

    /**
     * @brief Module service
     */
    ModuleService *_moduleService;
};


#endif // AWSMOCK_QT_UI_MAIN_WIDGET_H
