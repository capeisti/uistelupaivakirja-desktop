#ifndef TRIPCONTROLLER_H
#define TRIPCONTROLLER_H

#include <QObject>
#include <QStringList>
#include <QDate>
#include "controller.h"
#include "trip.h"
#include "htmlreport.h"



class TripController : public Controller
{
    Q_OBJECT
public:
    explicit TripController(QObject *parent = 0);

signals:

public:
    virtual void buttonEvent(EUISource source);

    virtual void booleanEvent(EUISource source, bool value);
    virtual void dateEvent(EUISource source, const QDate& value);
    virtual void textEvent(EUISource source, const QString& value);
    virtual void intEvent(EUISource source, int value);
    virtual void timeEvent(EUISource source, const QTime& value);

    virtual QDate getDateValue(EUISource source);
    virtual bool getBooleanValue(EUISource source);
    virtual int getIntValue(EUISource source);
    virtual QString getTextValue(EUISource source);
    virtual QTime getTimeValue(EUISource source);
    QMap<QString, int> getTripList();
    QList<QMap<QString, QString> > getFishList();
    QStringList getAlternatives(EUISource source);
    QList<QPair<QString, int> > getWayPointsList();
    QMap<QString, QString> getUserFields();
    QStringList getMediaFiles();
    QStringList getMediaFilesTrip();
    QString getTripReport();

protected:

private:
    void selectTrip(int);
    Trip* m_trip;
    HTMLReport m_report;
};

#endif // TRIPCONTROLLER_H
