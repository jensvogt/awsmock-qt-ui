//
// Created by vogje01 on 12/10/25.
//

#ifndef AWSMOCK_QT_UI_DTO_STATE_H
#define AWSMOCK_QT_UI_DTO_STATE_H
#include <QJsonObject>
#include <QString>

/**
 * @brief Docker container state
 *
 * @author jens.vogt\@opitz-consulting.com
 */
struct State {

    /**
     * Error
     */
    QString error;

    /**
     * Error code
     */
    int errorCode{};

    /**
     * PID
     */
    int pid{};

    /**
     * Running state
     */
    bool running{};

    /**
     * Dead state
     */
    bool dead{};

    /**
     * Paused state
     */
    bool paused{};

    /**
     * Restarting state
     */
    bool restarting{};

    /**
     * OOM killed state
     */
    bool oomKilled{};

    /**
     * Status
     */
    QString status;

    void FromJson(const QJsonObject &jsonObject) {
        error = jsonObject["Error"].toString();
        errorCode = jsonObject["ErrorCode"].toInt();
        pid = jsonObject["Pid"].toInt();
        running = jsonObject["Running"].toBool();
        dead = jsonObject["Dead"].toBool();
        paused = jsonObject["Paused"].toBool();
        restarting = jsonObject["Restarting"].toBool();
        oomKilled = jsonObject["OOMKilled"].toBool();
        status = jsonObject["Status"].toString();
    }
};

#endif //AWSMOCK_QT_UI_DTO_STATE_H
