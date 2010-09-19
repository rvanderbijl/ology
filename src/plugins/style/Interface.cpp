#include <QDebug>
#include <QtPlugin>
#include <QApplication>
#include <QPalette>

#include "Interface.h"

namespace Ology {
namespace Plugin {
namespace Style {

Interface::Interface() 
{
}


bool Interface::initialize(Ology::InitializePurpose initPurpose) {
    if (initPurpose == Ology::RealUsage) {
        qDebug() << "Setting sytle";
        /*
        QPalette p = QApplication::standardPalette();
        p.setColor( QPalette::Active, QPalette::Button, Qt::green );
        p.setBrush( QPalette::Inactive, QPalette::Button, Qt::green );
        QApplication::setStandardPalette(p);
        */
        qApp->setStyleSheet("QPushButton:focus { background: red; }");
    }
    return true;
}

}}}

Q_EXPORT_PLUGIN2("style", Ology::Plugin::Style::Interface)
