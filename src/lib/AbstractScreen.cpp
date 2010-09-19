#include "AbstractScreen.h"
#include "ManagerInterface.h"

namespace Ology {

void AbstractScreen::closeScreen() {
    if (OLOGY()->currentScreen()->id() == this->id()) {
        OLOGY()->closeCurrentScreen();
    }
}


}
