#include <QBuffer>
#include <QDebug>
#include <QSettings>
#include <QComboBox>

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

    emit variantValueChanged(value);
}

QString AbstractSetting::displayValue() const {
    const QVariant variant = variantValue();
    QString display = variant.toString();
    if (!display.isEmpty()) { return display; }

    QBuffer buf;
    buf.open(QIODevice::ReadWrite);
    QDataStream ds(&buf);
    const bool b = QMetaType::save(ds, variant.userType(), variant.constData());
    if (!b) {
        qDebug() << "Failed to create a displayValue for setting:" << untranslatedName();
    }

    return QString::fromLocal8Bit(buf.data().mid(4)); // skip QDataStream header 
}




}
