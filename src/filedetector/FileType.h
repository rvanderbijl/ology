#ifndef FILEDETECTOR_FILE_TYPE
#define FILEDETECTOR_FILE_TYPE

#include <QStringList>

namespace FileDetector {

/*! A FileType is used to describe the FileType FileDetector should search for
 *
 * Suffix may be empty, in which case all files are opened and examined 
 * (using libmagic) to determine if any of the mimeTypes match. The suffix
 * should simply be the extension, (e.g. to match README.txt, the suffix should be "txt").
 *
 * Comparision of FileTypes is case-sensitive, however searching can be either
 * case-sensitive or case-insensitive.
 *
 * mimeTypes may be empty, in which case only the suffix is examined.
 *
 * Each mimetype can end in "*", appropriate globbing will be applied.
 */
struct FileType {
    QString suffix;
    QStringList mimeTypes;
    bool operator==(const FileType & ei) {
        return this->suffix == ei.suffix &&
               this->mimeTypes == ei.mimeTypes;
    }
};


}

#endif
