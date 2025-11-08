cmake_minimum_required(VERSION 3.16)

project(awsmock-qt-ui VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 6.10 REQUIRED COMPONENTS Core Widgets Linguist Network Charts)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets Linguist Network Charts)

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
)


if (${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(awsmock-qt-ui
            MANUAL_FINALIZATION
            ${PROJECT_SOURCES}
            resources/awsmock-qt-ui.qrc
            include/mainwindow.h
            include/utils/BasePage.h
            include/utils/Configuration.h
            include/utils/EditPreferencesDialog.h
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
            include/modules/application/ApplicationList.h
            include/modules/application/ApplicationService.h
            src/utils/BasePage.cpp
            src/utils/Configuration.cpp
            src/utils/EditPreferencesDialog.cpp
            src/utils/RestManager.cpp
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
            src/modules/application/ApplicationList.cpp
            src/modules/application/ApplicationService.cpp
    )
    # Define target properties for Android with Qt 6 as:
    #    set_property(TARGET awsmock-qt-ui APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
    #                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
    # For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else ()
    if (ANDROID)
        add_library(awsmock-qt-ui SHARED
                ${PROJECT_SOURCES}
        )
        # Define properties for Android with Qt 5 after find_package() calls as:
        #    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else ()
        add_executable(awsmock-qt-ui
                ${PROJECT_SOURCES}
        )
    endif ()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif ()

add_subdirectory(src/utils)
add_subdirectory(src/modules/sqs)
add_subdirectory(src/modules/sns)

target_include_directories(awsmock-qt-ui PRIVATE ./include)
target_sources(awsmock-qt-ui PRIVATE src/main.cpp src/mainwindow.cpp)
target_link_libraries(awsmock-qt-ui PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Network
        Qt${QT_VERSION_MAJOR}::Charts)

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
install(TARGETS awsmock-qt-ui
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if (QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(awsmock-qt-ui)
endif ()
