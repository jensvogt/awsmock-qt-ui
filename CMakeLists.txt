cmake_minimum_required(VERSION 3.16)

project(awsmock-qt-ui VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

if (MSVC)
    set(CMAKE_PREFIX_PATH "c:\\work\\Qt\\6.10.0\\msvc2022_64")
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)
    set(CURL_INCLUDE_DIRS "C:\\work\\private\\extern\\curl\\include")
    set(CURL_LIB "$<$<CONFIG:RELEASE>:C:\\work\\private\\extern\\curl\\lib\\Release\\libcurl_imp.lib>"
            "$<$<CONFIG:DEBUG>:C:\\work\\private\\extern\\curl\\lib\\Debug\\libcurl-d_imp.lib>")
    set(FTP_INCLUDE_DIRS "C:\\work\\private\\extern\\ftpclient-cpp\\include")
    set(FTP_LIB "$<$<CONFIG:RELEASE>:C:\\work\\private\\extern\\ftpclient-cpp\\lib\\Release\\ftpclient.lib>"
            "$<$<CONFIG:DEBUG>:C:\\work\\private\\extern\\ftpclient-cpp\\lib\\Debug\\ftpclient.lib>")
    add_compile_options(/Zc:__cplusplus)
    set(WINDOWS_ICON ${CMAKE_SOURCE_DIR}/appicon.rc)
    set(APP_ICON_RESOURCE_WINDOWS "${WINDOWS_ICON}")
else ()
    set(CMAKE_PREFIX_PATH "~/work/Qt/6.10.0/gcc_64")
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CURL_INCLUDE_DIRS "")
    set(CURL_LIB_DIRS "")
endif ()

find_package(QT NAMES Qt6 6.10 REQUIRED COMPONENTS Core Widgets Linguist Network Charts Quick Graphs)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets Linguist Network Charts Quick Graphs)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Linguist Network Charts Quick Graphs)
# Ensure MSVC defines __cplusplus correctly

set(TS_FILES awsmock-qt-ui_en_150.ts)

set(PROJECT_SOURCES
        src/main.cpp
        src/mainwindow.cpp
        ${TS_FILES}
        src/modules/dashboard/DashboardService.cpp
        include/modules/application/ApplicationList.h
        src/modules/application/ApplicationList.cpp
        src/modules/application/ApplicationService.cpp
        include/dto/application/ApplicationListResponse.h
        src/modules/application/ApplicationAddDialog.cpp
        include/modules/application/ApplicationAddDialog.h
        include/utils/DropField.h
        include/utils/FileUtils.h
        include/dto/application/ApplicationUploadRequest.h
        include/dto/application/Application.h
        include/dto/application/ApplicationCreateRequest.h
        src/modules/application/ApplicationEditDialog.cpp
        include/modules/application/ApplicationEditDialog.h
        src/modules/application/ApplicationEditDialog.ui
        include/modules/application/ApplicationReloadDialog.h
        src/modules/application/ApplicationReloadDialog.cpp
        include/dto/sqs/SQSQueueCounter.h
        include/dto/sqs/SQSListQueueResponse.h
        include/dto/sqs/SQSMessageCounter.h
        include/modules/dashboard/CrossHairChartView.h
        include/modules/dashboard/CrossHairChartView.h
        include/modules/dashboard/CrossHairChartView.h
        include/modules/infrastructure/InfrastructureService.h
        include/modules/infrastructure/InfrastructureService.h
        src/modules/infrastructure/InfrastructureService.cpp
        src/modules/ftp/FTPUploadDialog.cpp
        include/modules/ftp/FTPUploadDialog.h
        src/modules/ftp/FTPUploadDialog.ui
        include/utils/NonEmptyValidator.h
        include/utils/NonEmptyValidator.h
        include/modules/s3/S3ObjectList.h
        include/dto/s3/S3ListObjectResponse.h
        include/dto/s3/S3ObjectCounter.h
        include/utils/About.h
        src/modules/application/ApplicationUploadCodeDialog.cpp
        include/modules/application/ApplicationUploadCodeDialog.h
        src/modules/application/ApplicationUploadCodeDialog.ui
        src/modules/application/ApplicationUploadCodeDialog.cpp
        include/utils/TableUtils.h
        src/utils/TableUtils.cpp
        src/utils/BaseDialog.cpp
        src/utils/BasePage.cpp
        src/modules/application/ApplicationEnvironmentEditDialog.cpp
        include/modules/application/ApplicationEnvironmentEditDialog.h
        src/modules/application/ApplicationEnvironmentEditDialog.ui
        src/modules/application/ApplicationTagDialog.cpp
        include/modules/application/ApplicationTagDialog.h
        src/modules/application/ApplicationTagDialog.ui
        include/modules/application/ApplicationTagDialog.h
        src/modules/application/ApplicationDependencyDialog.cpp
        include/modules/application/ApplicationDependencyDialog.h
        src/modules/application/ApplicationDependencyDialog.ui
        src/modules/sqs/SQSMessageAddDialog.cpp
        include/modules/sqs/SQSMessageAddDialog.h
        src/modules/sqs/SQSMessageAddDialog.ui
        include/utils/BaseService.h
        include/modules/s3/S3Service.h
        include/modules/s3/S3BucketList.h
        include/dto/s3/S3BucketCounter.h
        include/dto/s3/S3ListBucketResult.h
        src/modules/s3/S3BucketList.cpp
        src/modules/s3/S3Service.cpp
        src/modules/s3/S3BucketEditDialog.cpp
        include/modules/s3/S3BucketEditDialog.h
        src/modules/s3/S3BucketEditDialog.ui
        src/modules/s3/S3ObjectList.cpp
        src/utils/About.cpp
        src/utils/About.ui
        include/dto/gateway/GatewayConfig.h
        src/utils/EditConfigDialog.cpp
        include/utils/EditConfigDialog.h
        src/utils/EditConfigDialog.ui
        src/utils/RestManager.cpp
        include/dto/sqs/SQSQueueAttributeCounter.h
        include/dto/sqs/SQSQueueAttributeCounter.h
        include/dto/sqs/SQSListQueueAttributesResponse.h
        include/dto/sqs/SQSListQueueLambdaTriggersResponse.h
        include/dto/sqs/SQSQueueLambdaTriggerCounter.h
        include/dto/sqs/SQSSendMessageRequest.h
        include/dto/sqs/SQSMessageAttribute.h
        include/dto/sqs/SQSMessageAttributeDataType.h
        include/dto/sqs/SQSSendMessageResponse.h
        src/modules/sqs/SQSMessageAddDialog.cpp
        include/utils/JsonUtils.h
        include/utils/JsonUtils.h
        include/utils/ScopedTimer.h
        include/utils/EventBus.h
)

if (${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(awsmock-qt-ui
            MANUAL_FINALIZATION
            ${PROJECT_SOURCES}
            resources/awsmock-qt-ui.qrc
            include/mainwindow.h
            include/utils/BasePage.h
            include/utils/BaseDialog.h
            include/utils/TableUtils.h
            include/utils/Configuration.h
            include/utils/RestManager.h
            include/utils/JsonUtils.h
            include/dto/sns/SNSListTopicResult.h
            include/dto/sns/SNSTopicCounter.h
            include/dto/sns/SNSMessageList.h
            include/dto/sns/SNSListMessagesResult.h
            include/dto/sns/SNSMessageCounter.h
            include/dto/sns/SNSGetTopicDetailsResponse.h
            include/dto/sns/SNSGetMessageDetailsResponse.h
            include/dto/sqs/SQSGetQueueDetailsResponse.h
            include/dto/sqs/SQSGetMessageDetailsResponse.h
            include/dto/dashboard/DashboardCounterResult.h
            include/dto/application/ApplicationListResponse.h
            include/dto/application/ApplicationCounter.h
            include/modules/dashboard/Dashboard.h
            include/modules/dashboard/DashboardService.h
            include/modules/sqs/SQSQueueList.h
            include/modules/sqs/SQSMessageList.h
            include/modules/sqs/SQSService.h
            include/modules/sqs/SQSQueueDetailsDialog.h
            include/modules/sqs/SQSMessageDetailsDialog.h
            include/modules/sns/SNSTopicList.h
            include/modules/sns/SNSService.h
            include/modules/sns/SNSMessageList.h
            include/modules/sns/SNSTopicDetailsDialog.h
            include/modules/sns/SNSMessageDetailsDialog.h
            include/modules/s3/S3ObjectList.h
            include/modules/s3/S3BucketList.h
            include/modules/s3/S3Service.h
            include/modules/s3/S3BucketEditDialog.h
            include/modules/application/ApplicationList.h
            include/modules/application/ApplicationService.h
            include/modules/application/ApplicationUploadCodeDialog.h
            include/modules/infrastructure/InfrastructureService.h
            src/utils/BasePage.cpp
            src/utils/BaseDialog.cpp
            src/utils/Configuration.cpp
            src/utils/RestManager.cpp
            src/utils/TableUtils.cpp
            src/utils/IconUtils.cpp
            src/modules/dashboard/Dashboard.cpp
            src/modules/dashboard/Dashboard.ui
            src/modules/dashboard/DashboardService.cpp
            src/modules/sqs/SQSQueueList.cpp
            src/modules/sqs/SQSMessageList.cpp
            src/modules/sqs/SQSService.cpp
            src/modules/sqs/SQSQueueDetailsDialog.cpp
            src/modules/sqs/SQSQueueDetailsDialog.ui
            src/modules/sqs/SQSMessageDetailsDialog.ui
            src/modules/sqs/SQSMessageDetailsDialog.cpp
            src/modules/sns/SNSTopicList.cpp
            src/modules/sns/SNSService.cpp
            src/modules/sns/SNSMessageList.cpp
            src/modules/sns/SNSTopicDetailsDialog.ui
            src/modules/sns/SNSTopicDetailsDialog.cpp
            src/modules/sns/SNSMessageDetailsDialog.ui
            src/modules/sns/SNSMessageDetailsDialog.cpp
            src/modules/s3/S3BucketList.cpp
            src/modules/s3/S3ObjectList.cpp
            src/modules/s3/S3Service.cpp
            src/modules/s3/S3BucketEditDialog.cpp
            src/modules/s3/S3BucketEditDialog.ui
            src/modules/application/ApplicationList.cpp
            src/modules/application/ApplicationService.cpp
            src/modules/application/ApplicationUploadCodeDialog.cpp
            src/modules/application/ApplicationUploadCodeDialog.ui
            src/modules/infrastructure/InfrastructureService.cpp
            ${APP_ICON_RESOURCE_WINDOWS}
    )
    # Define target properties for Android with Qt 6 as:
    #    set_property(TARGET awsmock-qt-ui APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
    #                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
    # For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else ()
    if (ANDROID)
        add_library(awsmock-qt-ui SHARED ${PROJECT_SOURCES})
        # Define properties for Android with Qt 5 after find_package() calls as:
        #    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else ()
        add_executable(awsmock-qt-ui ${PROJECT_SOURCES})
    endif ()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif ()

target_include_directories(awsmock-qt-ui PRIVATE ./include ${CURL_INCLUDE_DIRS} ${FTP_INCLUDE_DIRS})
target_sources(awsmock-qt-ui PRIVATE src/main.cpp src/mainwindow.cpp)
if (MSVC)
    target_link_libraries(awsmock-qt-ui PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Network
            Qt${QT_VERSION_MAJOR}::Charts ${FTP_LIB} ${CURL_LIB})
else ()
    target_link_libraries(awsmock-qt-ui PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Network
            Qt${QT_VERSION_MAJOR}::Charts ftpclient curl)
endif ()

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if (${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.awsmock-qt-ui)
endif ()
set_target_properties(awsmock-qt-ui PROPERTIES
        ${BUNDLE_ID_OPTION}
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
if (MSVC)
    install(TARGETS awsmock-qt-ui BUNDLE DESTINATION . LIBRARY DESTINATION ${CMAKE_INSTALL_DIR}
            RUNTIME DESTINATION ${CMAKE_INSTALL_DIR})
else ()
    install(TARGETS awsmock-qt-ui BUNDLE DESTINATION . LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
    # Generate the deployment script for the target MyApp.
    qt_generate_deploy_app_script(
            TARGET awsmock-qt-ui
            OUTPUT_SCRIPT deploy_script
            NO_UNSUPPORTED_PLATFORM_ERROR)
    install(SCRIPT ${deploy_script})
endif ()

if (QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(awsmock-qt-ui)
endif ()
