########## UiModule Begin ##########

UIMODULE_DIR = $$PWD

SOURCES += \
    $$UIMODULE_DIR/uiCheckbox.cpp \
    $$UIMODULE_DIR/uiPushbutton.cpp \
    $$UIMODULE_DIR/uiSlideVerify.cpp \
    $$UIMODULE_DIR/uilabel.cpp \
    $$UIMODULE_DIR/uiwidget.cpp

HEADERS += \
    $$UIMODULE_DIR/uiCheckbox.h \
    $$UIMODULE_DIR/uiImageTextMixin.h \
    $$UIMODULE_DIR/uiPushbutton.h \
    $$UIMODULE_DIR/uiSlideVerify.h \
    $$UIMODULE_DIR/uilabel.h \
    $$UIMODULE_DIR/uiwidget.h

INCLUDEPATH += $$UIMODULE_DIR

########## UiModule End   ##########
