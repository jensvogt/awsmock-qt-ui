cmake_minimum_required(VERSION 3.16)

# Read version.txt
include(cmake/ReadVersion.cmake)

# Define project with version
project(awsmock-qt-ui VERSION ${VERSION_STRING} LANGUAGES CXX)

message(STATUS "Configured version: ${PROJECT_VERSION}")

# Export version to source code
configure_file(${CMAKE_SOURCE_DIR}/cmake/version.h.in ${CMAKE_SOURCE_DIR}/include/Version.h)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CPACK_PACKAGE_VERSION "${VERSION_STRING}")

if (WIN32)
    add_compile_options(/Zc:__cplusplus)
    set(CMAKE_PREFIX_PATH "c:\\work\\Qt\\6.10.0\\msvc2022_64")
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)
    set(CURL_INCLUDE_DIRS "extern/include")
    set(CURL_LIB "$<$<CONFIG:RELEASE>:${CMAKE_SOURCE_DIR}/extern/libcurl_imp.lib>"
            "$<$<CONFIG:DEBUG>:${CMAKE_SOURCE_DIR}/extern/libcurl-d_imp.lib>")
    set(WINDOWS_ICON ${CMAKE_SOURCE_DIR}/appicon.rc)
    set(APP_ICON_RESOURCE_WINDOWS "${WINDOWS_ICON}")
else ()
    set(CMAKE_PREFIX_PATH "~/work/Qt/6.10.0/gcc_64")
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CURL_INCLUDE_DIRS "")
    set(CURL_LIB_DIRS "")
endif ()


find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Svg Network Charts Svg)

set(TS_FILES awsmock-qt-ui_en_150.ts)

set(PROJECT_SOURCES
        src/main.cpp
        src/mainwindow.cpp
        include/mainwindow.h
        ${TS_FILES}
        src/modules/dashboard/Dashboard.cpp
        include/modules/dashboard/Dashboard.h
        src/modules/dashboard/Dashboard.ui
        src/modules/dashboard/DashboardService.cpp
        include/modules/dashboard/DashboardService.h
        include/modules/dashboard/ChartConfig.h
        include/modules/dashboard/CrossHairChartView.h
        src/modules/infrastructure/InfrastructureService.cpp
        include/modules/infrastructure/InfrastructureService.h
        src/modules/application/ApplicationList.cpp
        include/modules/application/ApplicationList.h
        src/modules/application/ApplicationService.cpp
        include/modules/application/ApplicationService.h
        src/modules/application/ApplicationAddDialog.cpp
        include/modules/application/ApplicationAddDialog.h
        src/modules/application/ApplicationEditDialog.cpp
        src/modules/application/ApplicationEditDialog.ui
        include/modules/application/ApplicationEditDialog.h
        src/modules/application/ApplicationReloadDialog.cpp
        include/modules/application/ApplicationReloadDialog.h
        src/modules/application/ApplicationDependencyDialog.cpp
        include/modules/application/ApplicationDependencyDialog.h
        src/modules/application/ApplicationUploadCodeDialog.cpp
        src/modules/application/ApplicationUploadCodeDialog.ui
        include/modules/application/ApplicationUploadCodeDialog.h
        src/modules/application/ApplicationUploadCodeDialog.cpp
        include/modules/application/ApplicationUploadCodeDialog.h
        src/modules/application/ApplicationEnvironmentEditDialog.cpp
        src/modules/application/ApplicationEnvironmentEditDialog.ui
        include/modules/application/ApplicationEnvironmentEditDialog.h
        src/modules/application/ApplicationTagDialog.cpp
        src/modules/application/ApplicationTagDialog.ui
        include/modules/application/ApplicationTagDialog.h
        src/modules/application/ApplicationDependencyDialog.cpp
        src/modules/application/ApplicationDependencyDialog.ui
        include/modules/application/ApplicationDependencyDialog.h
        src/modules/application/ApplicationLogsDialog.cpp
        src/modules/application/ApplicationLogsDialog.ui
        include/modules/application/ApplicationLogsDialog.h
        src/modules/ftp/FTPUploadDialog.cpp
        src/modules/ftp/FTPUploadDialog.ui
        include/modules/ftp/FTPUploadDialog.h
        include/modules/ftp/FTPClient.h
        include/modules/ftp/CurlHandle.h
        src/modules/sqs/SQSMessageAddDialog.cpp
        src/modules/sqs/SQSMessageAddDialog.ui
        include/modules/sqs/SQSMessageAddDialog.h
        src/modules/sqs/SQSMessageDetailsDialog.cpp
        src/modules/sqs/SQSMessageDetailsDialog.ui
        include/modules/sqs/SQSMessageDetailsDialog.h
        src/modules/sqs/SQSQueueDetailsDialog.cpp
        src/modules/sqs/SQSQueueDetailsDialog.ui
        include/modules/sqs/SQSQueueDetailsDialog.h
        src/modules/sqs/SQSService.cpp
        include/modules/sqs/SQSService.h
        src/modules/sqs/SQSQueueList.cpp
        include/modules/sqs/SQSQueueList.h
        src/modules/sqs/SQSMessageList.cpp
        include/modules/sqs/SQSMessageList.h
        src/modules/sns/SNSMessageDetailsDialog.cpp
        src/modules/sns/SNSMessageDetailsDialog.ui
        include/modules/sns/SNSMessageDetailsDialog.h
        src/modules/sns/SNSMessageList.cpp
        include/modules/sns/SNSMessageList.h
        src/modules/sns/SNSService.cpp
        include/modules/sns/SNSService.h
        src/modules/sns/SNSTopicDetailsDialog.cpp
        src/modules/sns/SNSTopicDetailsDialog.ui
        include/modules/sns/SNSTopicDetailsDialog.h
        src/modules/sns/SNSTopicList.cpp
        include/modules/sns/SNSTopicList.h
        src/modules/s3/S3BucketList.cpp
        include/modules/s3/S3BucketList.h
        src/modules/s3/S3Service.cpp
        include/modules/s3/S3Service.h
        src/modules/s3/S3BucketEditDialog.cpp
        src/modules/s3/S3BucketEditDialog.ui
        include/modules/s3/S3BucketEditDialog.h
        src/modules/s3/S3ObjectList.cpp
        include/modules/s3/S3ObjectList.h
        src/modules/s3/S3ObjectEditDialog.cpp
        src/modules/s3/S3ObjectEditDialog.ui
        include/modules/s3/S3ObjectEditDialog.h
        src/modules/docker/DockerLogClient.cpp
        include/modules/docker/DockerLogClient.h
        src/utils/About.cpp
        src/utils/About.ui
        include/utils/About.h
        include/utils/About.h
        src/utils/Configuration.cpp
        include/utils/Configuration.h
        src/utils/EditConfigDialog.cpp
        src/utils/EditConfigDialog.ui
        include/utils/EditConfigDialog.h
        src/utils/RestManager.cpp
        include/utils/RestManager.h
        src/utils/TableUtils.cpp
        include/utils/TableUtils.h
        src/utils/IconUtils.cpp
        include/utils/IconUtils.h
        include/utils/EventBus.h
        src/utils/BaseDialog.cpp
        include/utils/BaseDialog.h
        src/utils/BasePage.cpp
        include/utils/BasePage.h
        include/utils/NonEmptyValidator.h
        src/utils/IconUtils.cpp
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
        src/modules/ftp/FTPClient.cpp
        src/modules/ftp/CurlHandle.cpp
        src/modules/ftp/CurlHandle.cpp
        cmake/version.h.in
        src/modules/lambda/LambdaEnvironmentDetailDialog.cpp
        include/modules/lambda/LambdaEnvironmentDetailDialog.h
        src/modules/lambda/LambdaEnvironmentDetailDialog.ui
        src/modules/s3/S3ObjectMetadataDetailsDialog.cpp
        include/modules/s3/S3ObjectMetadataDetailsDialog.h
        src/modules/s3/S3ObjectMetadataDetailsDialog.ui
        src/modules/lambda/LambdaLogsDialog.cpp
        include/modules/lambda/LambdaLogsDialog.h
        src/modules/lambda/LambdaLogsDialog.ui
        include/dto/lambda/LambdaListResultsResponse.h
        include/dto/lambda/LambdaResultCounter.h
        src/modules/lambda/LambdaResultListDialog.cpp
        include/modules/lambda/LambdaResultListDialog.h
        src/modules/lambda/LambdaResultListDialog.ui
)

qt_add_resources(APP_RESOURCES resources/awsmock-qt-ui.qrc)
qt_add_executable(awsmock-qt-ui MANUAL_FINALIZATION ${PROJECT_SOURCES} ${APP_ICON_RESOURCE_WINDOWS} ${APP_RESOURCES})

target_include_directories(awsmock-qt-ui PRIVATE ./include ${CURL_INCLUDE_DIRS})
if (WIN32)
    target_link_directories(awsmock-qt-ui PRIVATE ${CMAKE_SOURCE_DIR}/extern/lib)
    target_link_libraries(awsmock-qt-ui PRIVATE Qt6::Widgets Qt6::Network Qt6::Charts Qt6::Svg libcurl_imp)
else ()
    target_link_libraries(awsmock-qt-ui PRIVATE Qt6::Widgets Qt6::Network Qt::Charts Qt6::Svg curl)
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
