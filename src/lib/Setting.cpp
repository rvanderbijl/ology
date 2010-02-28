#include <QSettings>

#include "Setting.h"

namespace Ology {

QVariant AbstractSetting::variantValue() const {
    const QString path = root() + "/" + name();
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "ology.org", "ology");
    return settings.contains(path) ? settings.value(path) : this->defaultVariantValue();
}

void AbstractSetting::setVariantValue(const QVariant &value) {
    const QString path = root() + "/" + name();
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "ology.org", "ology");
    settings.setValue(path, value);
}

}
