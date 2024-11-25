CONFIG      += qscintilla2
CONFIG      += debug_and_release
macx {
    QMAKE_POST_LINK = install_name_tool -change libqscintilla2_qt$${QT_MAJOR_VERSION}.15.dylib $$[QT_INSTALL_LIBS]/libqscintilla2_qt$${QT_MAJOR_VERSION}.15.dylib $(TARGET)
}
QT += widgets
QT += core gui widgets
QT += core5compat

INCLUDEPATH += $$PWD/Qsci

CONFIG(debug,debug|release) {
      LIBS += -L$$PWD -lqscintilla2_qt6d
} else {
      LIBS += -L$$PWD -lqscintilla2_qt6
}

HEADERS      = mainwindow.h \
    codeeditor.h \
    compile_setting.h \
    debugger.h \
    file_compile_setting.h \
    search.h
SOURCES      = main.cpp mainwindow.cpp \
    codeeditor.cpp \
    compile_setting.cpp \
    debugger.cpp \
    file_compile_setting.cpp \
    search.cpp
RESOURCES    = application.qrc

RC_FILE = icon.rc

FORMS += \
    file_compile_setting.ui

DISTFILES += \
    images/copy.png \
    images/cut.png \
    images/icon/Basic math.png \
    images/icon/JAVA.png \
    images/icon/NewProject.png \
    images/icon/add.png \
    images/icon/arrow_right.png \
    images/icon/c++.png \
    images/icon/caution.png \
    images/icon/check.png \
    images/icon/close.png \
    images/icon/cob.png \
    images/icon/coc.png \
    images/icon/compile box-line.png \
    images/icon/copy.png \
    images/icon/cos.png \
    images/icon/cut.png \
    images/icon/debug.png \
    images/icon/download.png \
    images/icon/edit.png \
    images/icon/exe.png \
    images/icon/file-open.png \
    images/icon/file.png \
    images/icon/filetree_dir.png \
    images/icon/filetree_file.png \
    images/icon/filter.png \
    images/icon/format-horizontal-align-bottom.png \
    images/icon/gui.png \
    images/icon/gui_window.png \
    images/icon/help.png \
    images/icon/icon.ico \
    images/icon/icon.png \
    images/icon/icon2.ico \
    images/icon/icon2.png \
    images/icon/icon_white.png \
    images/icon/info.png \
    images/icon/international.png \
    images/icon/list.png \
    images/icon/message.png \
    images/icon/my_gui_icon.png \
    images/icon/next.png \
    images/icon/nn.png \
    images/icon/nn_start.png \
    images/icon/nn_stop.png \
    images/icon/notification.png \
    images/icon/opening_icon .ico \
    images/icon/opening_icon.png \
    images/icon/opening_icon2.png \
    images/icon/opening_pic.png \
    images/icon/option.png \
    images/icon/order.png \
    images/icon/page_turning_right.png \
    images/icon/paste.png \
    images/icon/play.png \
    images/icon/print.png \
    images/icon/project.png \
    images/icon/py.png \
    images/icon/redo.png \
    images/icon/save.png \
    images/icon/search.png \
    images/icon/setting-filling.png \
    images/icon/star.png \
    images/icon/stop.png \
    images/icon/style.png \
    images/icon/style_choose.png \
    images/icon/time.png \
    images/icon/tmp.png \
    images/icon/txt.png \
    images/icon/undo.png \
    images/icon/unknown.png \
    images/icon/user.png \
    images/new.png \
    images/open.png \
    images/paste.png \
    images/save.png
