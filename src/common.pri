CONFIG -= release
CONFIG += debug rtti
INCLUDEPATH += $$PWD/include

!contains(TARGET,OlogyLib) {
    LIBS += -L$$PWD -lOlogy
}

OBJECTS_DIR = $$PWD/build/$$TARGET/objects
MOC_DIR     = $$PWD/build/$$TARGET/moc
UI_DIR      = $$PWD/build/$$TARGET/ui

DESTDIR = $$PWD

