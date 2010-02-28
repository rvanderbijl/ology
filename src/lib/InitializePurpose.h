#ifndef OLOGY_INITIALIZE_PURPOSE
#define OLOGY_INITIALIZE_PURPOSE

#include <QMetaType>

namespace Ology {

enum InitializePurpose {
    Introspection,
    RealUsage
};

};

Q_DECLARE_METATYPE(Ology::InitializePurpose);

#endif
