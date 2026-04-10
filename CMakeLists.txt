cmake_minimum_required(VERSION 3.19)
project(uiPushButton LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

# 启用 AUTORCC 自动处理 .qrc 文件
set(CMAKE_AUTORCC ON)

qt_add_executable(uiPushButton
    WIN32 MACOSX_BUNDLE
    main.cpp
    uiTestWidget.cpp
    uiTestWidget.h
    uiTestWidget.ui
    uiPushbutton.cpp uiPushbutton.h
    uiCheckbox.cpp uiCheckbox.h
    res.qrc
    uilabel.h uilabel.cpp
    uiwidget.h uiwidget.cpp
)

target_link_libraries(uiPushButton
    PRIVATE
        Qt::Core
        Qt::Widgets
)
target_link_libraries(uiPushButton PRIVATE Qt6::Core Qt6::Widgets)
target_link_libraries(uiPushButton PRIVATE Qt6::Core Qt6::Widgets)

include(GNUInstallDirs)

install(TARGETS uiPushButton
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET uiPushButton
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
