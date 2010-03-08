#ifndef FILEDETECTOR_CRITERIA
#define FILEDETECTOR_CRITERIA

#include <QFlags>

namespace FileDetector {

enum FileCriteria {
    FileReadable = 0x01,
    FileWritable = 0x02
};

enum VerifyCriteria {
    VerifyFileExists   = 0x01,
    VerifyFileCriteria = 0x02,
    VerifyMimeType     = 0x04
};

enum SearchCriteria {
    FollowSymLinks = 0x01,
    Recursive      = 0x02,
    IgnoreCase     = 0x04
};

Q_DECLARE_FLAGS(FileCriteriaFlags, FileCriteria);
Q_DECLARE_FLAGS(VerifyCriteriaFlags, VerifyCriteria);
Q_DECLARE_FLAGS(SearchCriteriaFlags, SearchCriteria);

};

Q_DECLARE_OPERATORS_FOR_FLAGS(FileDetector::FileCriteriaFlags);
Q_DECLARE_OPERATORS_FOR_FLAGS(FileDetector::VerifyCriteriaFlags);
Q_DECLARE_OPERATORS_FOR_FLAGS(FileDetector::SearchCriteriaFlags);

#endif
