#ifndef FILEDETECTOR_SEARCH
#define FILEDETECTOR_SEARCH

#include <QObject>
#include <QMetaType>
#include <QUrl>
#include "SearchParameters.h"

namespace FileDetector {
    class WorkerThreadController;

class Search : public QObject {
    Q_OBJECT
public:
    enum Error {
        ErrorWorkerThreadNotRunning,
        ErrorAddingSearchToWorkerThread,
    };

    Search(const SearchParameters & parameters, QObject *parent);

    const SearchParameters& searchParameters() const { return _parameters; }

    static QString errorString(Error code);

public slots:
    void startOneTimeSearch(WorkerThreadController *threadController);
    void startContinuousSearch();

signals:
    void searchError(FileDetector::Search::Error error);
    void oneTimeSearchStarted();
    void oneTimeSearchCompleted();

    void continuousSearchStarted();
    void continuousSearchVerified();

    void progress(int i, int total);
    void filesAdded(const QList<QUrl> &files);
    void filesRemoved(const QList<QUrl> &files);

protected: // allow search-worker access to it
    Search(const Search* search);
    SearchParameters _parameters;
};

}

Q_DECLARE_METATYPE(QList<QUrl>);

#endif
