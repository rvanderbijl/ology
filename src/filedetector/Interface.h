#ifndef FILEDETECTOR_INTERFACE
#define FILEDETECTOR_INTERFACE

#include <QThread>
#include <QUrl>

#include "Search.h"

namespace FileDetector {
    class WorkerThreadController;

class Interface : public QObject {
    Q_OBJECT
public:
    /*! Interface is the public interface for FileDetector. 
     * FileDetector works in its own thread. You can manually start() it, or it will be started
     * when the first search or directory-monitor is started. It is also started when the
     * storage-file is set. 
     */
    Interface(QObject *parent);

    /*! If the FileDetector thread is still running, the deconstructor will wait for it to stop.
     */
    ~Interface();


    /*! Returns true iff the worker thread is running.
     */
    bool isWorkerThreadRunning() const { return ((QThread*)_threadController)->isRunning(); }


    /*! Set the storage file where files found in monitored directories are stored
     *
     * If the file eixsts, the file will be opened, and all previously defined 
     * search-keys and found files (in monitored directories) are loaded. Call
     * verify() to verify that these files still exist and match the search criteria.
     *
     * This function is asynchronous. The signal loadFinished() is emitted when completed.
     */
    void setStorageFile( const QString &fileName );
    //QString storageFile() const { return _storageFileName; }


private:
    friend class Search;
    WorkerThreadController *_threadController;
};

}

#endif
