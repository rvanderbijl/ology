#include <Ology/AbstractScreen>
#include <Ology/CloseScreenAction>

#include <QStringList>
#include <QDebug>

#include "Manager.h"

namespace Ology {
namespace SettingsEditor {

Manager::Manager(int &argc, char** argv) :
    QApplication(argc, argv)
{
    setOrganizationName("ology.org");
    setApplicationName("ology");

    _window.fillInValues();
    _window.show();
}




}}
