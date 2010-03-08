#ifndef FILEDETECTOR_SEARCH
#define FILEDETECTOR_SEARCH

#include <QObject>
#include <QUrl>
#include "SearchParameters.h"

namespace FileDetector {
    class Interface;
    class Worker;

class Search : public QObject {
public:
    enum Error {
        ErrorNonInterfaceParent,
        ErrorWorkerThreadNotRunning,
        ErrorAddingSearchToWorkerThread,
    };

    Search(const SearchParameters & parameters, Interface *interface);

    const SearchParameters& searchParameters() const { return _parameters; }

    static QString errorString(Error code);

public slots:
    void startOneTimeSearch();
    void startContinuousSearch();

signals:
    void searchError(Error error);
    void oneTimeSearchStarted();
    void oneTimeSearchCompleted();

    void continuousSearchStarted();
    void continuousSearchVerified();

    void progress(int i, int total);
    void filesAdded(const QString &key, const QList<QUrl> &files);
    void filesRemoved(const QString &key, const QList<QUrl> &files);

protected: // allow search-worker access to it
    Search(const Search* search);
    SearchParameters _parameters;
};




}

#endif
