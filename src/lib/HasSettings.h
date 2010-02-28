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
    void setSettings(const QList<AbstractSetting*> &list) { _settings = list; }

private:
    QList<AbstractSetting*> _settings;
};




}

#endif
