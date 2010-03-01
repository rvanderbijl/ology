#ifndef OLOGY_HAS_SETTING
#define OLOGY_HAS_SETTING

#include <QVariant>
#include <QObject>

namespace Ology {
    class AbstractSetting;

class HasSettings {
public:
    virtual QList<AbstractSetting*> settings() const { return _settings; }

protected:
    void registerSetting(AbstractSetting* setting) { _settings.append(setting); }

private:
    QList<AbstractSetting*> _settings;
};




}

#endif
