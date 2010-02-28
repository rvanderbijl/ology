#include <QApplication>

#include "Manager.h"

int main(int argc, char** argv) {
    Ology::SettingsEditor::Manager app(argc, argv);
    return app.exec();
}

