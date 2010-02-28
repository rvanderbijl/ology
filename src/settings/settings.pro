TARGET=ology-settingseditor
include(../common.pri)
TEMPLATE=app

SOURCES += main.cpp

FORMS += SettingsEditor.ui
SOURCES += Manager.cpp SettingsEditor.cpp
HEADERS += Manager.h   SettingsEditor.h  

