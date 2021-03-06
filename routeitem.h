#ifndef ROUTEITEM_H
#define ROUTEITEM_H

#include <QLabel>
#include <QUrl>

class RouteItem : public QLabel
{
    Q_OBJECT
public:
    explicit RouteItem(QLabel *parent = 0);
    explicit RouteItem(const QString& text, QLabel *parent = 0);
    virtual void setText(const QString &);
    void insertFile(const QUrl& file);

signals:

public slots:

protected:
    virtual void dragEnterEvent ( QDragEnterEvent * event );
    virtual void dropEvent ( QDropEvent * event );
    virtual void dragMoveEvent(QDragMoveEvent *event);

};

#endif // ROUTEITEM_H
