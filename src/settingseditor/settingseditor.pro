TARGET=ology-settingseditor
include(../common.pri)
TEMPLATE=app

RESOURCES = SettingsEditor.qrc

SOURCES += main.cpp

FORMS   += SettingsEditor.ui
SOURCES += SettingsEditor.cpp ItemDelegate.cpp Manager.cpp TreeWidget.cpp
HEADERS += SettingsEditor.h   ItemDelegate.h   Manager.h   TreeWidget.h

