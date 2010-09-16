#ifndef OLOGY_GLOBAL_ACTION_IDS
#define OLOGY_GLOBAL_ACTION_IDS

#include <QString>

namespace Ology {
namespace Id {

namespace Action {
    const QString Quit("core-action-quit");
    const QString CloseScreen("core-action-close-screen");

    const QString Up("core-action-up");
    const QString Down("core-action-down");
    const QString Left("core-action-left");
    const QString Right("core-action-right");

    const QString PageUp("core-action-page-up");
    const QString PageDown("core-action-page-down");

    const QString First("core-action-first");
    const QString Last("core-action-last");

    const QString Play("core-action-play");
    const QString Stop("core-action-stop");
    const QString Next("core-action-next");
    const QString Prev("core-action-prev");
}


namespace Screen {
    const QString ConfirmQuit("core-screen-confirm-quit");
}


}}

#endif
