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

    void OnConnected() const;

    void OnMessageReceived(const QString &message) const;

    void SetupStatusbar();

private:
    /**
     * @brief UI Components
     */
    Ui::MainWidget *_ui;

    QWebSocket _webSocket;

    /**
     *  @brief Navigation item data model
     */
    QStandardItemModel *_navDataModel{};

    /**
     * @brief Data proxy model
     */
    PrefixFilterProxyModel *_proxyModel{};

    /**
     *  @brief Log item data model
     */
    QStandardItemModel *_logDataModel{};

    /**
     * @brief Content pane
     */
    QStackedWidget *_contentPane{};

    /**
     * @brief Scrolling flag
     */
    bool _scrolling = true;

    /**
     * @brief Websocket URL
     */
    QString _websocketUrl;

    QTimer *_reconnectTimer;
};


#endif // AWSMOCK_QT_UI_MAIN_WIDGET_H
