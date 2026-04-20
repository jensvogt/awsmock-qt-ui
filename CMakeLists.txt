cmake_minimum_required(VERSION 3.16)

# Read version.txt
include(cmake/ReadVersion.cmake)
include(cmake/UpdateXmlVersion.cmake)
include(cmake/UpdateReleaseDate.cmake)

# Define project with version
project(awsmock-qt-ui VERSION ${VERSION_STRING} LANGUAGES CXX)

message(STATUS "Configured version: ${PROJECT_VERSION}")

# Export version to source code
configure_file(${CMAKE_SOURCE_DIR}/cmake/version.h.in ${CMAKE_SOURCE_DIR}/include/Version.h)
update_xml_version("${CMAKE_SOURCE_DIR}/installer/config/config.xml" "${VERSION_STRING}" "Version")
update_xml_version("${CMAKE_SOURCE_DIR}/installer/packages/de.jensvogt.awsmockqtui/meta/package.xml" "${VERSION_STRING}" "Version")
update_release_date("${CMAKE_SOURCE_DIR}/installer/packages/de.jensvogt.awsmockqtui/meta/package.xml")

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CPACK_PACKAGE_VERSION "${VERSION_STRING}")

if (WIN32)
    add_compile_options(/Zc:__cplusplus)
    set(CMAKE_PREFIX_PATH "c:\\work\\private\\Qt\\6.10.1\\msvc2022_64")
    set(CMAKE_CXX_STANDARD 23)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)
    set(WINDOWS_ICON ${CMAKE_SOURCE_DIR}/appicon.rc)
    set(APP_ICON_RESOURCE_WINDOWS "${WINDOWS_ICON}")
else ()
    set(CMAKE_PREFIX_PATH "~/work/Qt/6.10.1/gcc_64")
    set(CMAKE_CXX_STANDARD 23)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif ()


find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Svg Network Charts Svg WebSockets)

set(TS_FILES awsmock-qt-ui_en_150.ts)
set(COMPONENTS_SOURCES include/components/FolderFilterModel.h src/components/FTPFileTree.cpp
        include/components/FTPFileTree.h include/components/FileFilterModel.h src/components/LocalFileTree.cpp
        include/components/LocalFileTree.h src/components/PageableTable.cpp include/components/PageableTable.h
        src/components/PageableTable.ui src/components/PageableList.cpp include/components/PageableList.h
        src/components/PageableList.ui src/components/ReplaceWordDialog.cpp include/components/ReplaceWordDialog.h
        src/components/ReplaceWordDialog.ui src/components/ReplaceWidget.cpp include/components/ReplaceWidget.h
        src/components/ReplaceWidget.ui src/components/SearchField.cpp include/components/SearchField.h
        src/components/SearchField.ui include/components/ContextMenu.h include/components/SelectModules.h
        src/components/SelectModules.cpp src/components/SelectModules.ui)
set(DASHBOARD_SOURCES src/modules/dashboard/Dashboard.cpp include/modules/dashboard/Dashboard.h
        src/modules/dashboard/Dashboard.ui src/modules/dashboard/DashboardService.cpp
        include/modules/dashboard/DashboardService.h include/modules/dashboard/ChartConfig.h
        include/modules/dashboard/CrossHairChartView.h)
set(FTP_SOURCES src/modules/ftpclient/FTPClientDialog.cpp src/modules/ftpclient/FTPClientDialog.ui
        include/modules/ftpclient/FTPClientDialog.h include/modules/ftpclient/FTPClientThread.h
        include/modules/ftpclient/FTPLowLevelClient.h src/modules/ftpclient/FTPLowLevelClient.cpp
        src/modules/ftpclient/FTPClientThread.cpp include/modules/ftpclient/FTPInfoThread.h)
set(SQS_SOURCES src/modules/sqs/SQSMessageAddDialog.cpp src/modules/sqs/SQSMessageAddDialog.ui
        include/modules/sqs/SQSMessageAddDialog.h src/modules/sqs/SQSMessageDetailsDialog.cpp
        src/modules/sqs/SQSMessageDetailsDialog.ui include/modules/sqs/SQSMessageDetailsDialog.h
        src/modules/sqs/SQSQueueDetailsDialog.cpp src/modules/sqs/SQSQueueDetailsDialog.ui
        include/modules/sqs/SQSQueueDetailsDialog.h src/modules/sqs/SQSService.cpp include/modules/sqs/SQSService.h
        src/modules/sqs/SQSQueueList.cpp include/modules/sqs/SQSQueueList.h src/modules/sqs/SQSMessageList.cpp
        include/modules/sqs/SQSMessageList.h)
set(SNS_SOURCES src/modules/sns/SNSMessageDetailsDialog.cpp src/modules/sns/SNSMessageDetailsDialog.ui
        include/modules/sns/SNSMessageDetailsDialog.h src/modules/sns/SNSMessageList.cpp include/modules/sns/SNSMessageList.h
        src/modules/sns/SNSService.cpp include/modules/sns/SNSService.h src/modules/sns/SNSTopicDetailsDialog.cpp
        src/modules/sns/SNSTopicDetailsDialog.ui include/modules/sns/SNSTopicDetailsDialog.h
        src/modules/sns/SNSTopicList.cpp include/modules/sns/SNSTopicList.h)
set(S3_SOURCES src/modules/s3/S3BucketList.cpp include/modules/s3/S3BucketList.h src/modules/s3/S3Service.cpp
        include/modules/s3/S3Service.h src/modules/s3/S3BucketEditDialog.cpp src/modules/s3/S3BucketEditDialog.ui
        include/modules/s3/S3BucketEditDialog.h src/modules/s3/S3ObjectList.cpp include/modules/s3/S3ObjectList.h
        src/modules/s3/S3ObjectEditDialog.cpp src/modules/s3/S3ObjectEditDialog.ui include/modules/s3/S3ObjectEditDialog.h
        src/modules/s3/S3BucketLambdaNotificationDialog.cpp include/modules/s3/S3BucketLambdaNotificationDialog.h
        src/modules/s3/S3BucketLambdaNotificationDialog.ui include/dto/s3/S3PutBucketNotificationRequest.h
        include/dto/s3/S3TopicConfiguration.h include/dto/s3/S3QueueConfiguration.h include/dto/s3/S3FilterRule.h
        include/dto/s3/S3NotificationEventType.h include/dto/s3/S3PutBucketNotificationConfigurationRequest.h)
set(KMS_SOURCES src/modules/kms/KMSService.cpp include/modules/kms/KMSService.h include/dto/kms/KMSListKeysResponse.h
        include/dto/kms/KeyCounter.h include/modules/kms/KMSKeyList.h include/dto/kms/KMSListKeysRequest.h
        include/dto/kms/KeySpec.h include/dto/kms/KeyUsage.h include/dto/kms/KeyState.h src/modules/kms/KMSKeyList.cpp
        src/modules/kms/KMSKeyDialog.cpp include/modules/kms/KMSKeyDialog.h src/modules/kms/KMSKeyDialog.ui
        include/dto/kms/KMSGetKeyCounterResponse.h include/dto/kms/KMSUpdateKeyCounterRequest.h include/dto/kms/Origin.h)
set(APPLICATION_SOURCES src/modules/application/ApplicationList.cpp include/modules/application/ApplicationList.h
        src/modules/application/ApplicationService.cpp include/modules/application/ApplicationService.h
        src/modules/application/ApplicationAddDialog.cpp include/modules/application/ApplicationAddDialog.h
        src/modules/application/ApplicationEditDialog.cpp src/modules/application/ApplicationEditDialog.ui
        include/modules/application/ApplicationEditDialog.h src/modules/application/ApplicationReloadDialog.cpp
        include/modules/application/ApplicationReloadDialog.h src/modules/application/ApplicationDependencyDialog.cpp
        include/modules/application/ApplicationDependencyDialog.h src/modules/application/ApplicationUploadCodeDialog.cpp
        src/modules/application/ApplicationUploadCodeDialog.ui include/modules/application/ApplicationUploadCodeDialog.h
        src/modules/application/ApplicationUploadCodeDialog.cpp include/modules/application/ApplicationUploadCodeDialog.h
        src/modules/application/ApplicationEnvironmentDialog.cpp src/modules/application/ApplicationEnvironmentDialog.ui
        include/modules/application/ApplicationEnvironmentDialog.h src/modules/application/ApplicationTagDialog.cpp
        src/modules/application/ApplicationTagDialog.ui include/modules/application/ApplicationTagDialog.h
        src/modules/application/ApplicationDependencyDialog.cpp src/modules/application/ApplicationDependencyDialog.ui
        include/modules/application/ApplicationDependencyDialog.h src/modules/application/ApplicationLogsDialog.cpp
        src/modules/application/ApplicationLogsDialog.ui include/modules/application/ApplicationLogsDialog.h)
set(COGNITO_SOURCES include/modules/cognito/CognitoService.h include/modules/cognito/CognitoUserpoolList.h
        src/modules/cognito/CognitoUserpoolList.cpp include/dto/cognito/CognitoUserpoolListResponse.h
        include/dto/cognito/CognitoUserpool.h src/modules/cognito/CognitoService.cpp
        src/modules/cognito/CognitoAddUserpoolDialog.cpp include/modules/cognito/CognitoAddUserpoolDialog.h
        src/modules/cognito/CognitoAddUserpoolDialog.ui include/dto/cognito/CognitoAddUserpoolRequest.h
        include/dto/cognito/CognitoAddUserpoolResponse.h include/dto/cognito/CognitoDeleteUserpoolRequest.h)
set(DYNAMODB_SOURCES src/modules/dynamodb/DynamoDbExportTableDialog.cpp src/modules/dynamodb/DynamoDbExportTableDialog.ui
        include/modules/dynamodb/DynamoDbExportTableDialog.h include/modules/dynamodb/DynamoDbTableList.h
        include/modules/dynamodb/DynamoDbService.h include/dto/dynamodb/DynamoDbListTableResponse.h
        include/dto/dynamodb/DynamoDbTableCounter.h src/modules/dynamodb/DynamoDbService.cpp
        src/modules/dynamodb/DynamoDbTableList.cpp include/dto/dynamodb/DynamoDbCreateTableRequest.h
        include/dto/dynamodb/DynamoDbCreateTableResponse.h include/dto/dynamodb/DynamoDbAttribute.h
        include/dto/dynamodb/DynamoDbKeySchema.h src/modules/dynamodb/DynamoDbAddTableDialog.cpp
        include/modules/dynamodb/DynamoDbAddTableDialog.h src/modules/dynamodb/DynamoDbAddTableDialog.ui
        src/modules/dynamodb/DynamoDbAddAttributeDialog.cpp include/modules/dynamodb/DynamoDbAddAttributeDialog.h
        src/modules/dynamodb/DynamoDbAddAttributeDialog.ui include/modules/dynamodb/DynamoDbAddKeySchemaDialog.h
        src/modules/dynamodb/DynamoDbAddKeySchemaDialog.cpp include/dto/dynamodb/DynamoDbProvisionedThroughput.h
        src/modules/dynamodb/DynamoDbEditTableDialog.cpp include/modules/dynamodb/DynamoDbEditTableDialog.h
        src/modules/dynamodb/DynamoDbEditTableDialog.ui include/dto/dynamodb/DynamoDbDescribeTableResponse.h
        src/modules/dynamodb/DynamoDbItemList.cpp include/modules/dynamodb/DynamoDbItemList.h
        include/dto/dynamodb/DynamoDbListItemResponse.h include/dto/dynamodb/DynamoDbItem.h
        include/dto/dynamodb/DynamoDbExportItemsResponse.h src/modules/dynamodb/DynamoDbItemDialog.cpp
        src/modules/dynamodb/DynamoDbItemDialog.ui include/modules/dynamodb/DynamoDbItemDialog.h
        include/dto/dynamodb/DynamoDbGetItemResponse.h)
set(MODULE_SOURCES src/modules/module/ModuleService.cpp include/modules/module/ModuleService.h
        src/modules/module/ModuleExportDialog.cpp include/modules/module/ModuleExportDialog.h
        src/modules/module/ModuleExportDialog.ui
)
set(MAIN_SOURCES src/main.cpp src/mainwindow.cpp include/mainwindow.h src/MainWidget.cpp include/MainWidget.h
        src/MainWidget.ui include/MainRouter.h)
set(UTILS_SOURCES src/utils/About.cpp src/utils/About.ui include/utils/About.h include/utils/About.h
        src/utils/Configuration.cpp include/utils/Configuration.h src/utils/EditConfigDialog.cpp src/utils/EditConfigDialog.ui
        include/utils/EditConfigDialog.h src/utils/RestManager.cpp include/utils/RestManager.h src/utils/TableUtils.cpp
        include/utils/TableUtils.h src/utils/IconUtils.cpp include/utils/IconUtils.h include/utils/EventBus.h
        src/utils/BaseDialog.cpp include/utils/BaseDialog.h src/utils/BasePage.cpp include/utils/BasePage.h
        include/utils/NonEmptyValidator.h src/utils/IconUtils.cpp src/utils/ServerLogWidget.cpp
        include/utils/ServerLogWidget.h src/utils/ServerLogWidget.ui src/utils/ConfigAddUrlDialog.cpp
        include/utils/ConfigAddUrlDialog.h src/utils/ConfigAddUrlDialog.ui)
set(PROJECT_SOURCES ${TS_FILES} ${MAIN_SOURCES} ${KMS_SOURCES} ${SQS_SOURCES} ${S3_SOURCES} ${DASHBOARD_SOURCES}
        ${MODULE_SOURCES} ${APPLICATION_SOURCES} ${SNS_SOURCES} ${COGNITO_SOURCES} ${UTILS_SOURCES} ${FTP_SOURCES}
        ${COMPONENTS_SOURCES} ${DYNAMODB_SOURCES}
        src/modules/docker/DockerLogClient.cpp
        include/modules/docker/DockerLogClient.h
        include/modules/lambda/LambdaList.h
        include/modules/lambda/LambdaService.h
        include/dto/lambda/LambdaCounter.h
        src/modules/lambda/LambdaList.cpp
        src/modules/lambda/LambdaService.cpp
        src/modules/lambda/LambdaDetailsDialog.cpp
        include/modules/lambda/LambdaDetailsDialog.h
        src/modules/lambda/LambdaDetailsDialog.ui
        include/dto/lambda/LambdaGetResponse.h
        include/Version.h
        include/dto/lambda/LambdaListEnvironmentResponse.h
        cmake/version.h.in
        src/modules/lambda/LambdaEnvironmentDetailDialog.cpp
        include/modules/lambda/LambdaEnvironmentDetailDialog.h
        src/modules/lambda/LambdaEnvironmentDetailDialog.ui
        src/modules/s3/S3ObjectMetadataDialog.cpp
        include/modules/s3/S3ObjectMetadataDialog.h
        src/modules/s3/S3ObjectMetadataDialog.ui
        src/modules/lambda/LambdaLogsDialog.cpp
        include/modules/lambda/LambdaLogsDialog.h
        src/modules/lambda/LambdaLogsDialog.ui
        include/dto/lambda/LambdaListResultsResponse.h
        include/dto/lambda/LambdaResultCounter.h
        src/modules/lambda/LambdaResultListDialog.cpp
        include/modules/lambda/LambdaResultListDialog.h
        src/modules/lambda/LambdaResultListDialog.ui
        include/dto/lambda/LambdaGetResultResponse.h
        src/modules/docker/DockerStatsDialog.cpp
        src/modules/docker/DockerService.cpp
        include/modules/docker/DockerService.h
        include/modules/docker/DockerStatsDialog.h
        include/dto/docker/DockerStatsResponse.h
        src/modules/docker/DockerStatsDialog.ui
        include/dto/docker/DockerContainersResponse.h
        include/dto/docker/Container.h
        include/dto/docker/CpuUsage.h
        include/dto/docker/CpuStat.h
        include/dto/docker/ContainerStat.h
        include/dto/docker/MemoryStat.h
        include/utils/PrefixFilterModel.h
        src/utils/PrefixFilterModel.cpp
        include/dto/docker/State.h
        include/utils/StringUtils.h
        src/modules/s3/S3ObjectAddDialog.cpp
        include/modules/s3/S3ObjectAddDialog.h
        src/modules/s3/S3ObjectAddDialog.ui
        src/modules/s3/S3ObjectAddDialog.cpp
        include/utils/BaseService.h
        include/dto/s3/S3ObjectUploadRequest.h
        include/modules/secretsmanager/SecretList.h
        src/modules/secretsmanager/SecretsList.cpp
        include/modules/lambda/LambdaUploadCodeDialog.h
        src/modules/lambda/LambdaUploadCodeDialog.cpp
        src/modules/lambda/LambdaUploadCodeDialog.ui
        include/dto/lambda/LambdaUploadRequest.h
        include/dto/lambda/LambdaUpdateRequest.h
        src/modules/sns/SNSMessageAddDialog.cpp
        src/modules/sns/SNSMessageAddDialog.ui
        include/modules/sns/SNSMessageAddDialog.h
        include/dto/sns/SNSSendMessageRequest.h
        include/dto/sns/SNSMessageAttribute.h
        include/dto/sns/SNSSendMessageResponse.h
        include/modules/secretsmanager/SecretsManagerService.h
        src/modules/secretsmanager/SecretsManagerService.cpp
        include/dto/secretsmanager/SecretsManagerListSecretsResponse.h
        include/dto/secretsmanager/SecretCounter.h
        src/modules/secretsmanager/SecretsDetailsDialog.cpp
        include/modules/secretsmanager/SecretsDetailsDialog.h
        src/modules/secretsmanager/SecretsDetailsDialog.ui
        include/dto/secretsmanager/SecretGetResponse.h
        src/modules/secretsmanager/SecretAddDialog.cpp
        include/modules/secretsmanager/SecretAddDialog.h
        src/modules/secretsmanager/SecretAddDialog.ui
        include/dto/secretsmanager/SecretVersionCounter.h
        include/dto/secretsmanager/SecretGetVersionsResponse.h
        include/modules/ssm/SSMParameterList.h
        include/modules/ssm/SSMService.h
        include/dto/ssm/SSMParameterListResponse.h
        src/modules/ssm/SSMParameterList.cpp
        src/modules/ssm/SSMService.cpp
        include/dto/ssm/SSMParameterCounter.h
        include/dto/ssm/SSMParameterGetResponse.h
        src/modules/ssm/SSMParameterEditDialog.cpp
        src/modules/ssm/SSMParameterEditDialog.ui
        include/modules/ssm/SSMParameterEditDialog.h
        src/modules/ssm/SSMParameterAddDialog.cpp
        include/modules/ssm/SSMParameterAddDialog.h
        src/modules/ssm/SSMParameterAddDialog.ui
        include/dto/sns/SNSListTopicAttributesResponse.h
        include/dto/sns/SNSTopicAttribute.h
        include/dto/sns/SNSListTopicTagsResponse.h
        include/dto/sns/SNSTopicTag.h
        include/dto/sns/SNSTopicSubscription.h
        include/dto/sns/SNSListTopicSubscriptionsResponse.h
        include/utils/DateTimeUtils.h
        include/modules/s3/S3BucketMetadataDialog.h
        src/modules/s3/S3BucketMetadataDialog.cpp
        src/modules/s3/S3BucketLifecycleDialog.cpp
        include/modules/s3/S3BucketLifecycleDialog.h
        src/modules/s3/S3BucketLifecycleDialog.ui
        src/modules/module/ShowInfrastructure.cpp
        include/modules/module/ShowInfrastructure.h
        src/modules/module/ShowInfrastructure.ui
        include/dto/ssm/SSMParameterPutResponse.h
        src/modules/ssm/SSMParameterTagDialog.cpp
        include/modules/ssm/SSMParameterTagDialog.h
        src/modules/ssm/SSMParameterTagDialog.ui
        src/modules/module/ModuleService.cpp
        include/utils/ImageUtils.h
        include/utils/ImageUtils.h
        src/utils/ImageUtils.cpp
        include/modules/ssm/SSMParameterTagDialog.h
        src/modules/ssm/SSMParameterTagDialog.cpp
        include/dto/kms/KMSCreateKeyRequest.h
        include/utils/Logging.h
        include/utils/UpdateChecker.h
        include/utils/DroppableTreeView.h
        include/utils/NetworkUtils.h
        include/utils/AwsUtils.h
        include/dto/module/ListModuleNamesResponse.h
        src/modules/sqs/SQSQueueDefaultAttributeDialog.cpp
        include/modules/sqs/SQSQueueDefaultAttributeDialog.h
        src/modules/sqs/SQSQueueDefaultAttributeDialog.ui
        include/dto/lambda/LambdaListArnsResponse.h
        include/components/PlainTextEditor.h
        src/components/PlainTextEditor.cpp
)

qt_add_resources(APP_RESOURCES resources/awsmock-qt-ui.qrc)
qt_add_executable(awsmock-qt-ui MANUAL_FINALIZATION ${PROJECT_SOURCES} ${APP_ICON_RESOURCE_WINDOWS} ${APP_RESOURCES})

target_include_directories(awsmock-qt-ui PRIVATE ./include)
if (WIN32)
    target_link_directories(awsmock-qt-ui PRIVATE ${CMAKE_SOURCE_DIR}/extern/lib)
    target_link_libraries(awsmock-qt-ui PRIVATE Qt6::Widgets Qt6::Network Qt6::Charts Qt6::Svg Qt6::WebSockets)
else ()
    target_link_libraries(awsmock-qt-ui PRIVATE Qt6::Widgets Qt6::Network Qt::Charts Qt6::Svg Qt6::WebSockets)
endif ()

set_target_properties(awsmock-qt-ui PROPERTIES
        MACOSX_BUNDLE_BUNDLE_VERSION "${PROJECT_VERSION}"
        MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}"
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
)

if (WIN32)
    install(TARGETS awsmock-qt-ui BUNDLE DESTINATION . LIBRARY DESTINATION ${CMAKE_INSTALL_DIR}
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
else ()
    include(GNUInstallDirs)
    install(TARGETS awsmock-qt-ui BUNDLE DESTINATION . LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
    # Generate the deployment script for the target MyApp.
    qt_generate_deploy_app_script(
            TARGET awsmock-qt-ui
            OUTPUT_SCRIPT deploy_script
            NO_UNSUPPORTED_PLATFORM_ERROR)
    install(SCRIPT ${deploy_script})
endif ()

qt_finalize_executable(awsmock-qt-ui)
