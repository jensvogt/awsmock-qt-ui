
#include <modules/cognito/CognitoService.h>

void CognitoService::ListUserpools(const QString &prefix, const long pageSize, const long pageIndex, const QString &sortColumn, const int sortDirection) {
    QElapsedTimer timer;
    timer.start();

    QJsonObject jSorting;
    jSorting["sortDirection"] = sortDirection;
    jSorting["column"] = sortColumn;

    QJsonArray jSortingArray;
    jSortingArray.append(jSorting);

    QJsonObject jRequest = CreateBaseRequest();
    jRequest["prefix"] = prefix;
    jRequest["pageSize"] = static_cast<qlonglong>(pageSize);
    jRequest["pageIndex"] = static_cast<qlonglong>(pageIndex);
    jRequest["sortColumns"] = jSortingArray;
    const QJsonDocument requestDoc(jRequest);

    _restManager.post(GetBaseUrl(),
                      requestDoc.toJson(),
                      {
                          {"x-awsmock-target", "cognito-idp"},
                          {"x-awsmock-action", "list-user-pool-counters"},
                          {"content-type", "application/json"}
                      },
                      [this, timer](const bool success, const QByteArray &response, int, const QString &error) {
                          if (success) {
                              if (const QJsonDocument jsonDoc = QJsonDocument::fromJson(response); jsonDoc.isObject()) {
                                  CognitoUserpoolListResponse userpoolResponse;
                                  userpoolResponse.FromJson(jsonDoc);
                                  emit ListUserPoolsSignal(userpoolResponse);
                              } else {
                                  logWarning << "Response is not an object!";
                              }
                          } else {
                              logError << error;
                          }
                          emit EventBus::instance().TimerSignal("ListUserpools", timer.elapsed());
                      });
}
