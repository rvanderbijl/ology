#ifndef OLOGY_SETTING
#define OLOGY_SETTING

#include <QVariant>
#include <QObject>
#include <Ology/HasNameDescription>

class QItemEditorCreatorBase;

namespace Ology {

class AbstractSetting : public QObject, public HasNameDescription {
    Q_OBJECT
    USE_HAS_NAME_DESCRIPTION
public:
    AbstractSetting(QObject *parent = 0) : QObject(parent) {}
    AbstractSetting(const QString &root, const QString &id, const QVariant & defaultValue, 
                    const char* trContext = 0, const char*name = 0, const char *desc = 0, QObject *parent = 0) : 
        QObject(parent),
        HasNameDescription(trContext, name, desc),
        _root(root),
        _id(id),
        _defaultValue(defaultValue)
    {
    }

    QString root() const { return _root; }
    QString id() const { return _id; }

    void setRoot(const QString &root) { _root = root; }
    void setId(const QString &id) { _id = id; }

    virtual QItemEditorCreatorBase* editorCreator() { return NULL; }
    virtual QString displayValue() const;

    QVariant variantValue() const;
    QVariant defaultVariantValue() const { return _defaultValue; }

    virtual void setVariantValue(const QVariant &value);
    virtual void setDefaultVariantValue(const QVariant &defaultValue) { _defaultValue = defaultValue; }

    // typically, these functions are protected in HasNameDescription, but for settings, we want to expose the functionality
    // so that the untranslated settings name etc can be provided by the owner of the setting object
    void setTranslationContext(const char* context) { HasNameDescription::setTranslationContext(context); }
    void setUntranslatedName(const char *name) { HasNameDescription::setUntranslatedName(name); }
    void setUntranslatedDescription(const char *description) { HasNameDescription::setUntranslatedDescription(description); }

signals:
    void variantValueChanged(const QVariant &newValue);

private:
    QString _root;
    QString _id;
    QVariant _defaultValue;
};


template<class T>
class Setting : public AbstractSetting {
public:
    Setting(QObject *parent = 0) : AbstractSetting(parent) {}
    Setting(const QString &root, const QString &id, const T & defaultValue,
            const char* trContext = 0, const char*name = 0, const char *desc = 0, QObject *parent = 0) : 
        AbstractSetting(root, id, QVariant::fromValue(defaultValue), trContext, name, desc, parent) 
    {
    }
 
    T value() const { QVariant v = variantValue(); return v.value<T>(); }
    T defaultValue() const { QVariant v = defaultVariantValue(); return v.value<T>(); }

    void setValue(const T &value) { setVariantValue(QVariant::fromValue<T>(value)); }
    void setDefaultValue(const T &defaultValue) { setDefaultVariantValue(QVariant::fromValue<T>(defaultValue)); }
};

}

#endif
