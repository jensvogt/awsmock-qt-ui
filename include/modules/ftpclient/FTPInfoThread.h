#ifndef AWSMOCK_QT_UI_INFO_THREAD_H
#define AWSMOCK_QT_UI_INFO_THREAD_H

// C++ includes
#include <string>

// Qt includes
#include <QThread>
#include <QString>

class InfoThread : public QThread {
    Q_OBJECT

public:
    explicit InfoThread() = default;

    void sendInfo(const std::string &info) {
        emit emitInfo(QString::fromStdString(info).trimmed());
    }

signals:
    void emitInfo(QString);
};

#endif // AWSMOCK_QT_UI_INFO_THREAD_H
