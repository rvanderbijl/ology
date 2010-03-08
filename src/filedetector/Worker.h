#ifndef FILEDETECTOR_WORK_DISPATCHER
#define FILEDETECTOR_WORK_DISPATCHER

#include <QReadWriteLock>
#include <QObject>
#include <QQueue>
#include <QTimer>
#include <QUrl>
#include <QSqlDatabase>
#include <QHash>

// lib-magic and inotify
extern "C" {
#include <magic.h>
#include <sys/inotify.h>
}



namespace FileDetector {
   class SearchWorker;

class WorkDispatcher : public QObject {
    Q_OBJECT
public:
    WorkDispatcher();

    Q_INVOKABLE void addSearchWorker(FileDetector::SearchWorker* worker);

    /*
    void setStorageFile( const QString &fileName );

    Q_INVOKABLE void addSearchKey(const FileDetector::SearchKey & key);
    Q_INVOKABLE void removeSearchKey(const QString &keyId);

    Q_INVOKABLE void searchDirectory(const QDir &directory, FileDetector::SearchCriteriaFlags searchCriteria, const QStringList &keyIds );
    Q_INVOKABLE void monitorDirectory(const FileDetector::MonitoredDirectory &monitoredDirectory);
    Q_INVOKABLE void unmonitorDirectory(const FileDetector::MonitoredDirectory &monitoredDirectory);
    Q_INVOKABLE void verifyFiles(const FileDetector::MonitoredDirectory &monitoredDirectory);

signals:
    void searchKeysChanged(const QList<FileDetector::SearchKey> &list);

    void loadFinished();
    void verifyCompleted( const FileDetector::MonitoredDirectory &monitoredDirectory );
    void searchDirectoryCompleted( const QDir &directory, const QStringList &keyIds, const QList<QUrl> &files );

    void filesAdded(const QString &keyId, const QList<QUrl> &files);
    void filesRemoved(const QString &keyId, const QList<QUrl> &files);

public slots:
    void doWork();
    void doSearchWork();

private:
    QReadWriteLock _lock;
    QSqlDatabase _db;

    QHash<QString, SearchKey> _searchKeys;

    struct Job {
        Job() : type(None) {}
        enum Type { None, Verify, Search };
        Type type;
        MonitoredDirectory directory;

        QStringList mergedExtensions;
        QList<QUrl> found;
        QFileInfoList todo;
    };


    Job _currentJob;
    */

private:
    magic_t _magicCookie;
    QQueue<SearchWorker*> _jobQueue;
    SearchWorker* _currentJob;
    QTimer _nullTimer;
};

}

#endif
