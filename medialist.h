#ifndef MEDIALIST_H
#define MEDIALIST_H

#include <QListWidget>
#include <QStringList>
#include <QListWidgetItem>
#include <QContextMenuEvent>
#include <QAction>

class MediaList : public QListWidget
{
    Q_OBJECT
public:
    explicit MediaList(int, int, QWidget *parent = 0);

    void setMediaFiles(QStringList);
    void insertMediaFile();

private slots:
    void openFile( QListWidgetItem * item );

protected:
    virtual void dragEnterEvent ( QDragEnterEvent * event );
    virtual void dropEvent ( QDropEvent * event );
    virtual void dragMoveEvent(QDragMoveEvent *event);
    virtual void contextMenuEvent ( QContextMenuEvent * e );
private:
    bool checkMovieExtension(const QString&);
    bool checkImageExtension(const QString&);
    bool checkFileExtension(const QString&);
    QAction* m_removeAction;
    QAction* m_openAction;
    int m_addId;
    int m_removeId;
};

#endif // MEDIALIST_H
