#ifndef OLOGY_SETTING
#define OLOGY_SETTING

#include <QVariant>
#include <QObject>
#include <QComboBox>
#include <QMetaEnum>
#include <QDataStream>
#include <QDebug>
#include <QItemEditorFactory>

#include <Ology/HasNameDescription>


/*! \brief This makes QDataStream << and >> operators so that the enum can be used as a setting.
 * 
 * This uses the QMetaObject of the specified class. The enum must be registered using Q_ENUMS.
 * It should be used outside of all namespaces. 
 */
#define OLOGY_DECLARE_OPERATORS_FOR_ENUM(class, enum) \
    QDataStream &operator<<(QDataStream &out, const class::enum &value) { \
        int idx = class::staticMetaObject.indexOfEnumerator(#enum); \
        out << class::staticMetaObject.enumerator(idx).key(value); \
        return out; \
    } \
    QDataStream &operator>>(QDataStream &in, class::enum &value) { \
        int idx = class::staticMetaObject.indexOfEnumerator(#enum); \
        char *buf = NULL; \
        in >> buf; \
        value = (class::enum)class::staticMetaObject.enumerator(idx).keyToValue(buf); \
        delete buf; buf = NULL; \
        return in; \
    }


namespace Ology {

class AbstractSetting : public QObject, public HasNameDescription {
    Q_OBJECT
    USE_HAS_NAME_DESCRIPTION
    Q_PROPERTY(QVariant value READ variantValue)
public:
    AbstractSetting(QObject *parent = 0) : QObject(parent) {
        connect(this, SIGNAL(variantValueChanged(const QVariant&)), SIGNAL(valueChanged()));
    }
    AbstractSetting(const QString &root, const QString &id, const QVariant & defaultValue, 
                    const char* trContext = 0, const char*name = 0, const char *desc = 0, QObject *parent = 0) : 
        QObject(parent),
        HasNameDescription(trContext, name, desc),
        _root(root),
        _id(id),
        _defaultValue(defaultValue)
    {
        connect(this, SIGNAL(variantValueChanged(const QVariant&)), SIGNAL(valueChanged()));
    }

    QString root() const { return _root; }
    QString id() const { return _id; }

    void setRoot(const QString &root) { _root = root; }
    void setId(const QString &id) { _id = id; }

    virtual QString displayValue() const;
    virtual QWidget* createEditor(QWidget *parent) { Q_UNUSED(parent); return NULL; }
    virtual bool takeValueFromEditor(QWidget *editor) { Q_UNUSED(editor); return NULL; }

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
    void valueChanged();

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

    T operator=(const T & v) {
        setValue(v);
        return v;
    }

    operator T() const {
        return value();
    }

    virtual QWidget* createEditor(QWidget *parent);
    virtual bool takeValueFromEditor(QWidget *editor);
};


template<class T>
QWidget* Setting<T>::createEditor( QWidget * parent ) {
    // check if T is an enum:
    const int metaTypeId = qMetaTypeId<T>();
    const QString name = QString(QMetaType::typeName(metaTypeId)).section("::", -1, -1);
    const int metaEnumIdx = this->parent() == NULL ? -1 : this->parent()->metaObject()->indexOfEnumerator(name.toLocal8Bit());

    if (metaEnumIdx > -1) {
        QComboBox *cb = new QComboBox(parent);
        QMetaEnum metaEnum = this->parent()->metaObject()->enumerator(metaEnumIdx);
        for(int i = 0; i < metaEnum.keyCount(); i++) {
            const int enumValue = metaEnum.value(i);
            const QString enumName = metaEnum.valueToKey(enumValue);
            cb->addItem(enumName, QVariant::fromValue((T)enumValue));
            if (enumValue == this->value()) {
                cb->setCurrentIndex( cb->count() - 1);
            }
        }

        return cb;
    } else {
        return QItemEditorFactory::defaultFactory()->createEditor((QVariant::Type)qMetaTypeId<T>(), parent);
    }
}

template<class T>
bool Setting<T>::takeValueFromEditor(QWidget *editor) {
    // check if T is an enum:
    const int metaTypeId = qMetaTypeId<T>();
    const QString name = QString(QMetaType::typeName(metaTypeId)).section("::", -1, -1);
    const int metaEnumIdx = this->parent() == NULL ? -1 : this->parent()->metaObject()->indexOfEnumerator(name.toLocal8Bit());

    if (metaEnumIdx > -1) {
        QComboBox *cb = qobject_cast<QComboBox*>(editor);
        Q_ASSERT(cb);
        QVariant v = cb->itemData( cb->currentIndex() );
        if (v.userType() == metaTypeId) {
            setValue( v.value<T>() );
            return true;
        }
    } else {
        setVariantValue( editor->property(QItemEditorFactory::defaultFactory()->valuePropertyName((QVariant::Type)metaTypeId)) );
        return true;
    }

    return false;
}



}

Q_DECLARE_METATYPE(Ology::AbstractSetting*)

#endif
