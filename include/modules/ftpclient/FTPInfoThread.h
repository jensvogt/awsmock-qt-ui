#ifndef AWSMOCK_QT_UI_INFO_THREAD_H
#define AWSMOCK_QT_UI_INFO_THREAD_H

// C++ includes
#include <string>

// Qt includes
#include <QThread>
#include <QString>

class FTPInfoThread : public QThread {
    Q_OBJECT

public:
    /**
     * @brief Constructor
     */
    explicit FTPInfoThread() = default;

    /**
     * @brief Singleton instance
     *
     * @return singleton instance
     */
    static FTPInfoThread &instance() {
        static FTPInfoThread t;
        return t;
    }

    /**
     * @brief Sends an info message
     *
     * @param info info message
     */
    void sendInfo(const std::string &info) {
        emit emitInfo(QString::fromStdString(info).trimmed());
    }

signals:
    /**
     * @brief Logging signal
     */
    void emitInfo(QString);
};

#endif // AWSMOCK_QT_UI_INFO_THREAD_H
