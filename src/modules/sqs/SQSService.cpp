
#include<modules/sqs/SQSService.h>

SQSService::SQSService() : tableWidget(nullptr) {
    m_netManager = new QNetworkAccessManager(nullptr);
    url = QUrl(Configuration::instance().GetBaseUrl());
}

void SQSService::ListQueues(const QString &prefix, QTableWidget* tableWidget) {

    this->tableWidget = tableWidget;

    // Connect the callback
    connect(m_netManager, &QNetworkAccessManager::finished, this, &SQSService::HandleListResult);

    QHttpHeaders headers;
    headers.append("x-awsmock-target", "sqs");
    headers.append("x-awsmock-action", "list-queue-counters");
    headers.append("content-type", "application/json");

    QNetworkRequest request(url);
    request.setHeaders(headers);

    QJsonObject jSorting;
    jSorting["sortDirection"] = -1;
    jSorting["column"] = "attributes.approximateNumberOfMessages";

    QJsonArray jSortingArray;
    jSortingArray.append(jSorting);

    QJsonObject jRequest;
    jRequest["prefix"] = prefix;
    jRequest["pageSize"] = -1;
    jRequest["pageIndex"] = -1;
    jRequest["sortColumns"] = jSortingArray;
    QJsonDocument requestDoc(jRequest);

    m_netManager->post(request, requestDoc.toJson());
}

void SQSService::HandleListResult(QNetworkReply *reply) {

    if (reply->error() != QNetworkReply::NoError) {

        // Handle network or HTTP error
        QString errorMsg = QString("<b>Error:</b> Failed to fetch quote: %1").arg(reply->errorString());
        qWarning() << "Network Error:" << reply->errorString();

    } else {

        // Read all raw data
        QByteArray responseData = reply->readAll();

        // The API returns an array containing one object: [{"q":"quote text", "a":"author"}]
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);

        if (jsonDoc.isObject()) {

            tableWidget->setRowCount(0);
            tableWidget->setSortingEnabled(false); // stop sorting
            tableWidget->sortItems(-1);
            QJsonArray counterArray = jsonDoc["queueCounters"].toArray();
            for (auto r = 0; r < counterArray.count(); r++) {
                tableWidget->insertRow(r);
                tableWidget->setItem(r, 0, new QTableWidgetItem(counterArray.at(r)["queueName"].toString()));
                QTableWidgetItem *item1 = new QTableWidgetItem;
                item1->setData(Qt::EditRole, counterArray.at(r)["available"].toInt());
                tableWidget->setItem(r, 1, item1);
                QTableWidgetItem *item2 = new QTableWidgetItem;
                item2->setData(Qt::EditRole, counterArray.at(r)["invisible"].toInt());
                tableWidget->setItem(r, 2, item2);
                QTableWidgetItem *item3 = new QTableWidgetItem;
                item3->setData(Qt::EditRole, counterArray.at(r)["delayed"].toInt());
                tableWidget->setItem(r, 3, item3);
                QTableWidgetItem *item4 = new QTableWidgetItem;
                item4->setData(Qt::EditRole, counterArray.at(r)["size"].toDouble());
                tableWidget->setItem(r, 4, item4);
                QTableWidgetItem *item5 = new QTableWidgetItem;
                item5->setData(Qt::EditRole, counterArray.at(r)["created"].toString());
                tableWidget->setItem(r, 5, item5);
                QTableWidgetItem *item6 = new QTableWidgetItem;
                item6->setData(Qt::EditRole, counterArray.at(r)["modified"].toString());
                tableWidget->setItem(r, 6, item6);
                QTableWidgetItem *item7 = new QTableWidgetItem;
                item7->setData(Qt::EditRole, counterArray.at(r)["queueUrl"].toString());
                tableWidget->setItem(r, 7, item7);
                QTableWidgetItem *item8 = new QTableWidgetItem;
                item8->setData(Qt::EditRole, counterArray.at(r)["queueArn"].toString());
                tableWidget->setItem(r, 8, item8);
                QTableWidgetItem *checkItem = new QTableWidgetItem();
                checkItem->setCheckState(counterArray.at(r)["isDlq"].toBool()?Qt::Checked:Qt::Unchecked);
                checkItem->setFlags(checkItem->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
                tableWidget->setItem(r, 9, checkItem);
            }
            tableWidget->setRowCount(counterArray.count());
            tableWidget->setSortingEnabled(true);
        } else {
            //m_quoteLabel->setText("Error: Failed to parse API response.");
        }
    }

    // Clean up the reply object
    reply->deleteLater();
    disconnect(m_netManager, &QNetworkAccessManager::finished, this, &SQSService::HandleListResult);
}

void SQSService::PurgeQueue(QString QueueUrl){

    // Connect the callback
    connect(m_netManager, &QNetworkAccessManager::finished, this, &SQSService::HandlePurgeResult);

    QHttpHeaders headers;
    headers.append("x-awsmock-target", "sqs");
    headers.append("x-awsmock-action", "purge-queue");
    headers.append("content-type", "application/json");

    QNetworkRequest request(url);
    request.setHeaders(headers);

    QJsonObject jRequest;
    jRequest["QueueUrl"] = QueueUrl;
    QJsonDocument requestDoc(jRequest);

    m_netManager->post(request, requestDoc.toJson());
}

void SQSService::HandlePurgeResult(QNetworkReply *reply) {

    if (reply->error() == QNetworkReply::NoError) {
        emit LoadContent();
    }
    // Clean up the reply object
    reply->deleteLater();
    disconnect(m_netManager, &QNetworkAccessManager::finished, this, &SQSService::HandlePurgeResult);
}

void SQSService::PurgeAllQueues(){

    // Connect the callback
    connect(m_netManager, &QNetworkAccessManager::finished, this, &SQSService::HandleAllPurgeResult);

    QHttpHeaders headers;
    headers.append("x-awsmock-target", "sqs");
    headers.append("x-awsmock-action", "purge-all-queues");
    headers.append("content-type", "application/json");

    QNetworkRequest request(url);
    request.setHeaders(headers);

    m_netManager->post(request, nullptr);
}

void SQSService::HandleAllPurgeResult(QNetworkReply *reply) {

    if (reply->error() == QNetworkReply::NoError) {
        emit LoadContent();
    }
    // Clean up the reply object
    reply->deleteLater();
    disconnect(m_netManager, &QNetworkAccessManager::finished, this, &SQSService::HandleAllPurgeResult);
}

void SQSService::AddQueue(const QString &QueueName) {

    // Connect the callback
    connect(m_netManager, &QNetworkAccessManager::finished, this, &SQSService::HandleAddResult);

    QHttpHeaders headers;
    headers.append("x-awsmock-target", "sqs");
    headers.append("x-awsmock-action", "create-queue");
    headers.append("content-type", "application/json");

    QNetworkRequest request(url);
    request.setHeaders(headers);

    QJsonObject jRequest;
    jRequest["QueueName"] = QueueName;
    QJsonDocument requestDoc(jRequest);

    m_netManager->post(request, requestDoc.toJson());
}

void SQSService::HandleAddResult(QNetworkReply *reply) {

    if (reply->error() == QNetworkReply::NoError) {
        emit LoadContent();
    }
    // Clean up the reply object
    reply->deleteLater();
    disconnect(m_netManager, &QNetworkAccessManager::finished, this, &SQSService::HandleAddResult);
}

void SQSService::UpdateQueue(const UpdateQueueRequest &updateQueueRequest){

    // Connect the callback
    connect(m_netManager, &QNetworkAccessManager::finished, this, &SQSService::HandleUpdateResult);

    QHttpHeaders headers;
    headers.append("x-awsmock-target", "sqs");
    headers.append("x-awsmock-action", "update-queue");
    headers.append("content-type", "application/json");

    QNetworkRequest request(url);
    request.setHeaders(headers);

    m_netManager->post(request, updateQueueRequest.ToJson());
}

void SQSService::HandleUpdateResult(QNetworkReply *reply) {

    if (reply->error() != QNetworkReply::NoError) {
        qWarning() << "Update Queue failed";
    }
    // Clean up the reply object
    reply->deleteLater();
    disconnect(m_netManager, &QNetworkAccessManager::finished, this, &SQSService::HandleUpdateResult);
}

void SQSService::DeleteQueue(const QString &QueueUrl){

    // Connect the callback
    connect(m_netManager, &QNetworkAccessManager::finished, this, &SQSService::HandleDeleteResult);

    QHttpHeaders headers;
    headers.append("x-awsmock-target", "sqs");
    headers.append("x-awsmock-action", "delete-queue");
    headers.append("content-type", "application/json");

    QNetworkRequest request(url);
    request.setHeaders(headers);

    QJsonObject jRequest;
    jRequest["QueueUrl"] = QueueUrl;
    QJsonDocument requestDoc(jRequest);

    m_netManager->post(request, requestDoc.toJson());
}

void SQSService::HandleDeleteResult(QNetworkReply *reply) {

    if (reply->error() == QNetworkReply::NoError) {
        emit LoadContent();
    }
    // Clean up the reply object
    reply->deleteLater();
    disconnect(m_netManager, &QNetworkAccessManager::finished, this, &SQSService::HandleDeleteResult);
}

void SQSService::RedriveQueue(QString QueueArn){

    // Connect the callback
    connect(m_netManager, &QNetworkAccessManager::finished, this, &SQSService::HandleRedriveResult);

    QHttpHeaders headers;
    headers.append("x-awsmock-target", "sqs");
    headers.append("x-awsmock-action", "redrive-messages");
    headers.append("content-type", "application/json");

    QNetworkRequest request(url);
    request.setHeaders(headers);

    QJsonObject jRequest;
    jRequest["QueueArn"] = QueueArn;
    QJsonDocument requestDoc(jRequest);

    m_netManager->post(request, requestDoc.toJson());
}

void SQSService::HandleRedriveResult(QNetworkReply *reply) {

    if (reply->error() == QNetworkReply::NoError) {
        emit LoadContent();
    }
    // Clean up the reply object
    reply->deleteLater();
    disconnect(m_netManager, &QNetworkAccessManager::finished, this, &SQSService::HandleRedriveResult);
}

void SQSService::GetQueueDetails(QString QueueArn){

    // Connect the callback
    connect(m_netManager, &QNetworkAccessManager::finished, this, &SQSService::HandleGetQueueDetailResult);

    QHttpHeaders headers;
    headers.append("x-awsmock-target", "sqs");
    headers.append("x-awsmock-action", "get-queue-details");
    headers.append("content-type", "application/json");

    QNetworkRequest request(url);
    request.setHeaders(headers);

    QJsonObject jRequest;
    jRequest["QueueArn"] = QueueArn;
    QJsonDocument requestDoc(jRequest);

    m_netManager->post(request, requestDoc.toJson());
}

void SQSService::HandleGetQueueDetailResult(QNetworkReply *reply) {

    if (reply->error() != QNetworkReply::NoError) {

        // Handle network or HTTP error
        QString errorMsg = QString("<b>Error:</b> Failed to fetch quote: %1").arg(reply->errorString());
        qWarning() << "Network Error:" << reply->errorString();

    } else {

        // Read all raw data
        QByteArray responseData = reply->readAll();
        //qDebug() << QString(responseData);

        // The API returns an array containing one object
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
        SQSGetQueueDetailsResponse response;
        response.FromJson(jsonDoc);

        emit GetQueueDetailsSignal(response);
    }

    // Clean up the reply object
    reply->deleteLater();
    disconnect(m_netManager, &QNetworkAccessManager::finished, this, &SQSService::HandleGetQueueDetailResult);
}

void SQSService::GetSqsMessageDetails(QString messageId){

    QJsonObject jRequest;
    jRequest["messageId"] = messageId;
    QJsonDocument requestDoc(jRequest);

    _restManager.post(url,
                      requestDoc.toJson(),
                      {{"x-awsmock-target", "sqs"},
                       {"x-awsmock-action", "get-message-counters"},
                       {"content-type", "application/json"}},
                      [this](bool success, QByteArray response, int status, QString error) {
                          if (success) {
                              // The API returns an JSON object
                              QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
                              SQSGetMessageDetailsResponse response;
                              response.FromJson(jsonDoc["message"].toObject());
                              emit GetSqsMessageDetailsSignal(response);
                          } else {
                              QMessageBox::critical(nullptr, "Error", error);
                          }
                      });
}

void SQSService::ListMessages(const QString &queueArn, const QString &prefix, QTableWidget* tableWidget){

    // Copy table widget
    this->tableWidget = tableWidget;

    // Connect the callback
    connect(m_netManager, &QNetworkAccessManager::finished, this, &SQSService::HandleMessageListResult);

    QHttpHeaders headers;
    headers.append("x-awsmock-target", "sqs");
    headers.append("x-awsmock-action", "list-message-counters");
    headers.append("content-type", "application/json");

    QNetworkRequest request(url);
    request.setHeaders(headers);

    QJsonObject jSorting;
    jSorting["sortDirection"] = -1;
    jSorting["column"] = "created";

    QJsonArray jSortingArray;
    jSortingArray.append(jSorting);

    QJsonObject jRequest;
    jRequest["queueArn"] = queueArn;
    jRequest["prefix"] = prefix;
    jRequest["pageSize"] = -1;
    jRequest["pageIndex"] = -1;
    jRequest["sortColumns"] = jSortingArray;
    QJsonDocument requestDoc(jRequest);

    m_netManager->post(request, requestDoc.toJson());
    qDebug() << "POST request sent to:" << url << ", target: "<< headers.values("x-awsmock-target") << ", action: " << headers.values("x-awsmock-action");
}

void SQSService::HandleMessageListResult(QNetworkReply *reply) {

    if (reply->error() != QNetworkReply::NoError) {

        // Handle network or HTTP error
        QString errorMsg = QString("<b>Error:</b> Failed to fetch quote: %1").arg(reply->errorString());
        qWarning() << "Network Error:" << reply->errorString();

    } else {

        // Read all raw data
        QByteArray responseData = reply->readAll();
        //qDebug() << QString(responseData);

        // The API returns an array containing one object
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);

        if (jsonDoc.isObject()) {

            tableWidget->clearContents();
            tableWidget->setRowCount(0);
            tableWidget->setSortingEnabled(false); // stop sorting
            tableWidget->sortItems(-1);
            QJsonArray counterArray = jsonDoc["messageCounters"].toArray();
            for (auto r = 0; r < counterArray.count(); r++) {
                tableWidget->insertRow(r);
                tableWidget->setItem(r, 0, new QTableWidgetItem(counterArray.at(r)["messageId"].toString()));
                tableWidget->setItem(r, 1, new QTableWidgetItem(counterArray.at(r)["contentType"].toString()));

                QTableWidgetItem *item1 = new QTableWidgetItem;
                item1->setData(Qt::EditRole, counterArray.at(r)["size"].toInt());
                tableWidget->setItem(r, 2, item1);

                QTableWidgetItem *item2 = new QTableWidgetItem;
                item2->setData(Qt::EditRole, counterArray.at(r)["retires"].toInt());
                tableWidget->setItem(r, 3, item2);

                QTableWidgetItem *item3 = new QTableWidgetItem;
                item3->setData(Qt::EditRole, counterArray.at(r)["created"].toString());
                tableWidget->setItem(r, 4, item3);

                QTableWidgetItem *item4 = new QTableWidgetItem;
                item4->setData(Qt::EditRole, counterArray.at(r)["modified"].toString());
                tableWidget->setItem(r, 5, item4);
            }
            tableWidget->setRowCount(counterArray.count());
            tableWidget->setSortingEnabled(true);
        } else {
            //m_quoteLabel->setText("Error: Failed to parse API response.");
        }
    }

    // Clean up the reply object
    reply->deleteLater();
    disconnect(m_netManager, &QNetworkAccessManager::finished, this, &SQSService::HandleMessageListResult);
}

void SQSService::PurgeAllMesssages(const QString &QueueUrl){

    // Connect the callback
    connect(m_netManager, &QNetworkAccessManager::finished, this, &SQSService::HandleMessageAllPurgeResult);

    // API Endpoint: Get a random quote
    QUrl url("http://localhost:4566");

    QHttpHeaders headers;
    headers.append("x-awsmock-target", "sqs");
    headers.append("x-awsmock-action", "purge-Queue");
    headers.append("content-type", "application/json");

    QJsonObject jRequest;
    jRequest["QueueUrl"] = QueueUrl;
    QJsonDocument requestDoc(jRequest);

    QNetworkRequest request(url);
    request.setHeaders(headers);

    m_netManager->post(request, requestDoc.toJson());
}

void SQSService::HandleMessageAllPurgeResult(QNetworkReply *reply) {
    if (reply->error() == QNetworkReply::NoError) {
        emit LoadContent();
    }
    // Clean up the reply object
    reply->deleteLater();
    disconnect(m_netManager, &QNetworkAccessManager::finished, this, &SQSService::HandleAllPurgeResult);
}
