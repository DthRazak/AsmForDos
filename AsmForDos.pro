QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    core/converter.cpp \
    core/directive.cpp \
    core/format.cpp \
    core/operand.cpp \
    core/operandstore.cpp \
    core/operator.cpp \
    core/operatorstore.cpp \
    core/parser.cpp \
    main.cpp \
    ui/codeeditor.cpp \
    ui/compilationtable.cpp \
    ui/mainwindow.cpp

HEADERS += \
    core/converter.h \
    core/directive.h \
    core/exceptions.h \
    core/format.h \
    core/instruction.h \
    core/operand.h \
    core/operandstore.h \
    core/operator.h \
    core/operatorstore.h \
    core/parser.h \
    core/types.h \
    ui/codeeditor.h \
    ui/compilationtable.h \
    ui/mainwindow.h

FORMS += \
    ui/mainwindow.ui

# RESOURCES = application.qrc

RC_ICONS = exe_icon.ico

DISTFILES += \
    DOS/DOSBox.exe \
    DOS/SDL.dll \
    DOS/SDL_net.dll

install_it.path = %{buildDir}
install_it.files += %{sourceDir}/DOS/DOSBox.exe
install_it.files += %{sourceDir}/DOS/SDL.dll
install_it.files += %{sourceDir}/DOS/SDL_net.dll

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INSTALLS += install_it
