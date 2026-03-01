//
// Created by vogje01 on 2/13/26.
//

#ifndef AWSMOCK_QT_UI_SERVER_LOG_WIDGET_H
#define AWSMOCK_QT_UI_SERVER_LOG_WIDGET_H

// Qt includes
#include <QWidget>
#include <QWebSocket>

// Awsmock includes
#include <utils/BaseDialog.h>
#include <utils/IconUtils.h>

QT_BEGIN_NAMESPACE

namespace Ui {
    class ServerLogWidget;
}

QT_END_NAMESPACE

class ServerLogWidget : public BaseDialog {
    Q_OBJECT

public:
    explicit ServerLogWidget(QWidget *parent = nullptr);

    ~ServerLogWidget() override;

    void HandleAccept();

    void HandleReject();

    void OnConnected() const;

    void OnMessageReceived(const QString &message) const;

    void LoadContent() override {
    }

signals:
    /**
     * @brief Send when the websocket is connected
     */
    void WebsocketConnected();

    /**
     * @brief Send when the dialog is closed
     */
    void WebsocketClosed();

private:
    /**
     * @brief UI components
     */
    Ui::ServerLogWidget *_ui;

    /**
     * @brief Scrolling flag
     */
    bool _scrolling = true;
    /**
     *  @brief Log item data model
     */
    QStandardItemModel *_serverLogDataModel{};

    /**
     * @brief Websocket URL
     */
    QString _websocketUrl;

    /**
     * @brief Websocket for the server logs
     */
    QWebSocket *_webSocket{};

    /**
     * @brief Reconnect timer
     */
    QTimer *_reconnectTimer{};
};


#endif // AWSMOCK_QT_UI_SERVER_LOG_WIDGET_H
