TARGET=ology-settingseditor
include(../common.pri)
TEMPLATE=app

SOURCES += main.cpp

FORMS   += SettingsEditor.ui
SOURCES += SettingsEditor.cpp Manager.cpp TreeWidget.cpp
HEADERS += SettingsEditor.h   Manager.h   TreeWidget.h

