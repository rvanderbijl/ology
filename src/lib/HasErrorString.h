#ifndef OLOGY_HAS_ERROR_STRING
#define OLOGY_HAS_ERROR_STRING

#include <QString>

namespace Ology {

class HasErrorString {
public:
    QString errorString() const { return _errorString; }

protected:
    void setErrorString(const QString &string) { _errorString = string; }
    void clearErrorString() { _errorString.clear(); }

private:
    QString _errorString;
};

};


#endif
