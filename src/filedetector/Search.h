#ifndef FILEDETECTOR_SEARCH
#define FILEDETECTOR_SEARCH

#include <QObject>
#include <QMetaType>
#include <QUrl>
#include "SearchParameters.h"

namespace FileDetector {
    class WorkerThreadController;
    class SearchWorker;

class Search : public QObject {
    Q_OBJECT
public:
    enum Error {
        ErrorWorkerThreadNotRunning,
        ErrorAddingSearchToWorkerThread,
    };

    Search(const SearchParameters & parameters, WorkerThreadController * wtc, QObject *parent);
    ~Search();

    const SearchParameters& searchParameters() const { return _parameters; }
    WorkerThreadController *threadController() const { return _threadController; }

    static QString errorString(Error code);

public slots:
    void startSearch();
    void stopSearch();

signals:
    void searchError(FileDetector::Search::Error error);
    void progress(int i, int total);
    void filesAdded(const QList<QUrl> &files);
    void filesRemoved(const QList<QUrl> &files);

private:
    SearchParameters _parameters;
    WorkerThreadController *_threadController;
    SearchWorker *_worker;
};

}

Q_DECLARE_METATYPE(QList<QUrl>);

#endif
