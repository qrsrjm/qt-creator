TEMPLATE = lib
TARGET = Debugger

# CONFIG += single
include(../../qtcreatorplugin.pri)
include(../../plugins/projectexplorer/projectexplorer.pri)
include(../../plugins/find/find.pri)
include(../../plugins/coreplugin/coreplugin.pri)
include(../../plugins/texteditor/texteditor.pri)
include(../../plugins/cpptools/cpptools.pri)
include(../../libs/cplusplus/cplusplus.pri)
include(../../libs/utils/utils.pri)
INCLUDEPATH += $$PWD/../../libs/utils

QT += gui network script

HEADERS += \
    breakhandler.h \
    breakwindow.h \
    debuggeractions.h \
    debuggerconstants.h \
    debuggerdialogs.h \
    debuggermanager.h \
    debuggeroutputwindow.h \
    debuggerplugin.h \
    debuggerrunner.h \
    disassemblerhandler.h \
    disassemblerwindow.h \
    watchutils.h \
    idebuggerengine.h \
    imports.h \
    moduleshandler.h \
    moduleswindow.h \
    outputcollector.h \
    procinterrupt.h \
    registerhandler.h \
    registerwindow.h \
    scriptengine.h \
    stackhandler.h \
    stackwindow.h \
    sourcefileswindow.h \
    threadswindow.h \
    watchhandler.h \
    watchwindow.h \

SOURCES += \
    breakhandler.cpp \
    breakwindow.cpp \
    breakwindow.h \
    debuggeractions.cpp \
    debuggerdialogs.cpp \
    debuggermanager.cpp \
    debuggeroutputwindow.cpp \
    debuggerplugin.cpp \
    debuggerrunner.cpp \
    disassemblerhandler.cpp \
    disassemblerwindow.cpp \
    watchutils.cpp \
    moduleshandler.cpp \
    moduleswindow.cpp \
    outputcollector.cpp \
    procinterrupt.cpp \
    registerhandler.cpp \
    registerwindow.cpp \
    scriptengine.cpp \
    stackhandler.cpp \
    stackwindow.cpp \
    sourcefileswindow.cpp \
    threadswindow.cpp \
    watchhandler.cpp \
    watchwindow.cpp \

FORMS += attachexternaldialog.ui \
    attachcoredialog.ui \
    attachtcfdialog.ui \
    breakbyfunction.ui \
    breakcondition.ui \
    dumperoptionpage.ui \
    commonoptionspage.ui \
    startexternaldialog.ui \
    startremotedialog.ui \

RESOURCES += debugger.qrc

false {
SOURCES += $$PWD/modeltest.cpp
HEADERS += $$PWD/modeltest.h
DEFINES += USE_MODEL_TEST=1
}

include(gdb/gdb.pri)
include(tcf/tcf.pri)
include(win/win.pri)
include(cdb/cdb.pri)

OTHER_FILES += Debugger.pluginspec
