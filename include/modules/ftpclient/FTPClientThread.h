#ifndef AWSMOCK_QT_UI_CLIENT_THREAD_H
#define AWSMOCK_QT_UI_CLIENT_THREAD_H

// Qt includes
#include <QThread>

// Awsmock includes
#include <modules/ftpclient/FTPLowLevelClient.h>

class QStandardItem;

enum subThreadTask { TConnect, TDisconnect, TCd, TDown, TUp, TDele, TRmd, TRename, TMkd, TList };

class FTPClientThread : public QThread {
    Q_OBJECT

public:
    explicit FTPClientThread();

    ~FTPClientThread() override;

    subThreadTask task;
    std::vector<std::string> arglist = std::vector<std::string>(5);
    Client *curClient;
    QStandardItem *parent{};

protected:
    void run() override;

private:
    Client *client{};

    void flushList();

public slots:
    void stop();

signals:
    void emitListItem(QString, QString, QString);

    void emitFileListItem(const FileInfo &item, QStandardItem *parent);

    void emitInfo(QString);

    void emitSuccess();

    void emitClearList();
};


#endif // AWSMOCK_QT_UI_CLIENT_THREAD_H
