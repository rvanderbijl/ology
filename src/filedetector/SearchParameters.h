#ifndef FILEDETECTOR_SEARCH_KEY
#define FILEDETECTOR_SEARCH_KEY

#include <QList>
#include <QDir>
#include "FileType.h"
#include "Criteria.h"

namespace FileDetector {

struct SearchParameters {
    QDir directory;
    QList<FileType> fileTypes;
    FileCriteriaFlags fileCriteria;
    SearchCriteriaFlags searchCriteria;

    bool operator==(const SearchParameters & sp) {
        return this->directory      == sp.directory      &&
               this->fileCriteria   == sp.fileCriteria   &&
               this->searchCriteria == sp.searchCriteria &&
               this->fileTypes      == sp.fileTypes       ;
    }
};


}

#endif
