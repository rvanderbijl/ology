#include <QApplication>

#include "Manager.h"


int main(int argc, char** argv) {
    Ology::Manager app(argc, argv);
    app.displayScreen("menu-main");
    return app.exec();
}

