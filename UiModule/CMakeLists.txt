cmake_minimum_required(VERSION 3.19)

set(UIMODULE_SOURCES
    uiCheckbox.cpp
    uiPushbutton.cpp
    uiSlideVerify.cpp
    uilabel.cpp
    uiwidget.cpp
)

set(UIMODULE_HEADERS
    uiCheckbox.h
    uiImageTextMixin.h
    uiPushbutton.h
    uiSlideVerify.h
    uilabel.h
    uiwidget.h
)

add_library(UiModule STATIC ${UIMODULE_SOURCES} ${UIMODULE_HEADERS})

target_include_directories(UiModule PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(UiModule PUBLIC Qt6::Core Qt6::Widgets)
