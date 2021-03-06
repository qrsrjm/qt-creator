QTC_LIB_DEPENDS += qmljs
QTC_PLUGIN_DEPENDS += qmljstools

include(../../../qttest.pri)

DEFINES+=QTCREATORDIR=\\\"$$IDE_SOURCE_TREE\\\"
DEFINES+=TESTSRCDIR=\\\"$$PWD\\\"

QT += core
QT -= gui

CONFIG += c++11

TARGET = tst_dependencies
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += tst_dependencies.cpp

DISTFILES += \
    samples/test_ApplicationWindow.qml \
    samples/test_Button.qml \
    samples/test_ColumnLayout.qml \
    samples/test_Component.qml \
    samples/test_GridLayout.qml \
    samples/test_ListModel.qml \
    samples/test_ListView.qml \
    samples/test_LocalStorage.qml \
    samples/test_Material.qml \
    samples/test_MessageDialog.qml \
    samples/test_ObjectModel.qml \
    samples/test_QtObject.qml \
    samples/test_RowLayout.qml \
    samples/test_SampleLib.qml \
    samples/test_StackLayout.qml \
    samples/test_Tests.qml \
    samples/test_Timer.qml \
    samples/test_Universal.qml \
    samples/test_Window.qml \
    samples/test_XmlListModel.qml

