CONFIG -= release
CONFIG += debug rtti
INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD $$PWD/lib $$PWD/include $$PWD/include/Ology/ $$PWD/include/Ology/Core

!contains(TARGET,OlogyLib) {
    LIBS += -L$$PWD -lOlogy
}

OBJECTS_DIR = $$PWD/build/$$TARGET/objects
MOC_DIR     = $$PWD/build/$$TARGET/moc
UI_DIR      = $$PWD/build/$$TARGET/ui

DESTDIR = $$PWD

