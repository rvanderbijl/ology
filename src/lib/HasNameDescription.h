#ifndef OLOGY_HAS_NAME_DESC
#define OLOGY_HAS_NAME_DESC

#include <QApplication>

#define USE_HAS_NAME_DESCRIPTION \
public: \
    virtual QString name() const { return HasNameDescription::translatedName(); } \
    virtual QString description() const { return HasNameDescription::translatedDescription(); }

namespace Ology {

class HasNameDescription {
public:
    HasNameDescription() :
        _context(NULL),
        _untranslatedName(NULL),
        _untranslatedDescription(NULL)
    {}

    HasNameDescription(const char *context, const char *name, const char *desc) :
        _context(context),
        _untranslatedName(name),
        _untranslatedDescription(desc)
    {}


    /*! Return the transalated name for this object
     */
    virtual QString name() const = 0;
     
    /*! Return the transalated description for this object
     */
    virtual QString description() const = 0;

protected:
    void setTranslationContext(const char* context) { _context = context; }
    void setUntranslatedName(const char *name) { _untranslatedName = name; }
    void setUntranslatedDescription(const char *description) { _untranslatedDescription = description; }

    virtual QString translatedName() const { return QApplication::translate(_context, _untranslatedName); }
    virtual QString translatedDescription() const { return QApplication::translate(_context, _untranslatedDescription); }

    const char* translationContext() const { return _context; }
    const char* untranslatedName() const { return _untranslatedName; }
    const char* untranslatedDescription() const { return _untranslatedDescription; }

private:
    const char *_context;
    const char *_untranslatedName;
    const char *_untranslatedDescription;
};

}

#endif
