#ifndef OLOGY_SETTING
#define OLOGY_SETTING

#include <QVariant>
#include <QObject>

class QItemEditorCreatorBase;

namespace Ology {

class AbstractSetting : public QObject {
    Q_OBJECT
public:
    AbstractSetting(QObject *parent = 0) : QObject(parent) {}
    AbstractSetting(const QString &root, const QString &name, const QString &description, const QVariant & defaultValue, QObject *parent = 0) : 
        QObject(parent),
        _root(root),
        _name(name),
        _description(description),
        _defaultValue(defaultValue)
    {
    }

    QString root() const { return _root; }
    QString name() const { return _name; }
    QString description() const { return _description; }

    void setRoot(const QString &root) { _root = root; }
    void setName(const QString &name) { _name = name; }
    void setDescription(const QString &description) { _description = description; }

    virtual QItemEditorCreatorBase* editorCreator() { return NULL; }
    virtual QString displayValue() const;

    QVariant variantValue() const;
    QVariant defaultVariantValue() const { return _defaultValue; }

    virtual void setVariantValue(const QVariant &value);
    virtual void setDefaultVariantValue(const QVariant &defaultValue) { _defaultValue = defaultValue; }

private:
    QString _root;
    QString _name;
    QString _description;
    QVariant _defaultValue;
};


template<class T>
class Setting : public AbstractSetting {
public:
    Setting(QObject *parent = 0) : AbstractSetting(parent) {}
    Setting(const QString &root, const QString &name, const QString &description, const T & defaultValue, QObject *parent = 0) : 
        AbstractSetting(root, name, description, QVariant::fromValue(defaultValue), parent) 
    {
    }
 
    T value() const { QVariant v = variantValue(); return v.value<T>(); }
    T defaultValue() const { QVariant v = defaultVariantValue(); return v.value<T>(); }

    void setValue(const T &value) { setVariantValue(QVariant::fromValue<T>(value)); }
    void setDefaultValue(const T &defaultValue) { setDefaultVariantValue(QVariant::fromValue<T>(defaultValue)); }
};




}

#endif
