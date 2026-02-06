//
// Created by vogje01 on 2/6/26.
//

#ifndef AWSMOCK_QT_UI_MAIN_ROUTER_H
#define AWSMOCK_QT_UI_MAIN_ROUTER_H

// Qt includes
#include <QMap>
#include <QString>
#include <QWidget>
#include <QStackedWidget>

// Awsmock includes
#include <utils/BasePage.h>
#include <modules/dashboard/Dashboard.h>
#include <modules/sqs/SQSQueueList.h>
#include <modules/sqs/SQSMessageList.h>
#include <modules/sns/SNSTopicList.h>
#include <modules/sns/SNSMessageList.h>
#include <modules/s3/S3BucketList.h>
#include <modules/s3/S3ObjectList.h>
#include <modules/application/ApplicationList.h>
#include <modules/lambda/LambdaList.h>
#include <modules/secretsmanager/SecretList.h>
#include <modules/ssm/SSMParameterList.h>
#include <modules/dynamodb/DynamoDbTableList.h>
#include <modules/dynamodb/DynamoDbItemList.h>

class MainRouter : public QWidget {
    Q_OBJECT

public:
    /**
     * @brief Constructor
     *
     * @param parent parent widget
     */
    explicit MainRouter(QWidget *parent = nullptr) : _contentPane(nullptr) {
        routes = {
            {
                "Dashboard", new Dashboard("Dashboard", parent)
            },
            {
                "SQS", new SQSQueueList("SQS Queue List", parent)
            },
            {
                "SQS Message List", new SQSMessageList("SQS Message List", parent)
            },
            {
                "SNS", new SNSTopicList("SNS Topic List", parent)
            },
            {
                "SNS Message List", new SNSMessageList("SNS Message List", parent)
            },
            {
                "S3", new S3BucketList("S3 Bucket List", parent)
            },
            {
                "S3 Object List", new S3ObjectList("S3 Object List", parent)
            },
            {
                "Application", new ApplicationList("Application List", parent)
            },
            {
                "Lambda", new LambdaList("Lambda List", parent)
            },
            {
                "Secrets Manager", new SecretList("Secrets List", parent)
            },
            {
                "Systems Manager", new SSMParameterList("Parameter List", parent)
            },
            {
                "DynamoDB", new DynamoDbTableList("DynamoDB Table List", parent)
            },
            {
                "DynamoDB Item List", new DynamoDbItemList("DynamoDB Item List", parent)
            }
        };
        connect(&EventBus::instance(), &EventBus::RouteChanged, this, [this](const QString &pageName, const QMap<QString, QString> &arguments) {
            if (routes.contains(pageName)) {
                SetRoute(pageName, arguments);
            }
        });
    }

    /**
     * @brief Singleton instance
     *
     * @return
     */
    static MainRouter &instance() {
        static MainRouter instance;
        return instance;
    }

    /**
     * @brief Destructor
     */
    ~MainRouter() override = default;

    void SetContentPane(QStackedWidget *contentPane) {
        _contentPane = contentPane;
    }

    void SetRoute(const QString &pageName, const QMap<QString, QString> &arguments = {}) {

        // Setup content pane
        if (routes.contains(pageName)) {


            // Set arguments
            BasePage *basePage = routes[pageName];
            basePage->SetArguments(arguments);

            // Set content
            _contentPane->addWidget(basePage);
            _contentPane->setCurrentWidget(basePage);
            _contentPane->update();

            // Start auto updater
            basePage->StartAutoUpdate();
        }
    }

signals:
    void RouterChangedSignal(const BasePage *basePage);

private slots:
    void ChangeRoute(const QString &pageName, const QMap<QString, QString> &arguments) {
        if (routes.contains(pageName)) {

            // Set content
            _contentPane->addWidget(routes[pageName]);
            _contentPane->setCurrentWidget(routes[pageName]);
            _contentPane->update();

            // Start auto updater
            routes[pageName]->StartAutoUpdate();
        }
    }

private:
    /**
     * @brief Routine map
     */
    QMap<QString, BasePage *> routes;

    /**
     * @brief Content pane
     */
    QStackedWidget *_contentPane;
};

#endif //AWSMOCK_QT_UI_MAIN_ROUTER_H
