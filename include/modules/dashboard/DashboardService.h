#ifndef DASHBOARD_SERVICE_H
#define DASHBOARD_SERVICE_H

#include <QMessageBox>
#include <QObject>
#include <utils/Configuration.h>
#include <utils/RestManager.h>
#include <utils/JsonUtils.h>

class DashboardService final : public QObject
{
    Q_OBJECT

public:

    /**
     * @brief SNSService
     */
    DashboardService();

    /**
     * @brief Get a multi series counters
     *
     * @param region AWS region
     * @param name counter name
     * @param labelName label name
     * @param labelValue label value
     * @param step number of step
     * @param limit top n limit
     * @param start start time
     * @param end end time
     */
    void GetMultiSeriesCounter(const QString &region, const QString &name, const QString &labelName,
                               const QString &labelValue, int step, int limit, const QDateTime &start,
                               const QDateTime &end);

signals:
/*    void ListTopicSignal(const SNSListTopicResult &listTopicResult);
    void ListMessagesSignal(const SNSListMessagesResult &listMessagesResult);
    void GetTopicDetailsSignal(const SNSGetTopicDetailsResponse &response);
    void GetMessageDetailsSignal(const SNSGetMessageDetailsResponse &response);
    void ReloadMessagesSignal();*/

private:

    /**
     * @brief HTTP REST manager
     */
    RestManager _restManager;

    /**
     * @brief Base URL
     */
    QUrl url;
};


#endif // DASHBOARD_SERVICE_H
