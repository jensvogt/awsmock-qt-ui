#ifndef AWSMOCK_QT_UI_CLIENT_THREAD_H
#define AWSMOCK_QT_UI_CLIENT_THREAD_H

// Qt includes
#include <QThread>

// Awsmock includes
#include <modules/ftpclient/FTPLowLevelClient.h>

enum subThreadTask { TConnect, TDisconnect, TCd, TDown, TUp, TDele, TRmd, TRename, TMkd };

class FTPClientThread : public QThread {
    Q_OBJECT

public:
    explicit FTPClientThread();

    ~FTPClientThread() override;

    void bind(Client *c);

    subThreadTask task;
    std::vector<std::string> arglist;
    Client *curClient;

protected:
    void run() override;

private:
    Client *client;

    void flushList();

public slots:
    void stop();

signals:
    void emitListItem(QString, QString, QString);

    void emitFileListItem(const FileInfo &item);

    void emitInfo(QString);

    void emitSuccess();

    void emitClearList();
};


#endif // AWSMOCK_QT_UI_CLIENT_THREAD_H
