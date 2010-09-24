#ifndef FILEDETECTOR_SEARCH_KEY
#define FILEDETECTOR_SEARCH_KEY

#include <QList>
#include <QDir>
#include "FileType.h"
#include "Criteria.h"

namespace FileDetector {

struct SearchParameters {
    SearchParameters() :
        fileCriteria(FileReadable),
        searchCriteria(FollowSymLinks|Recursive),
        verifyCriteria(VerifyFileExists|VerifyFileCriteria|VerifyMimeType),
        monitorForChanges(false)
    {}

    QDir directory;
    QList<FileType> fileTypes;
    FileCriteriaFlags fileCriteria;
    SearchCriteriaFlags searchCriteria;
    VerifyCriteriaFlags verifyCriteria;
    bool monitorForChanges;

    bool operator==(const SearchParameters & sp) {
        return this->directory        == sp.directory      &&
               this->fileCriteria     == sp.fileCriteria   &&
               this->searchCriteria   == sp.searchCriteria &&
               this->verifyCriteria   == sp.verifyCriteria &&
               this->fileTypes        == sp.fileTypes      &&
               this->monitorForChanges == sp.monitorForChanges;
    }
};


}

#endif
