#ifndef TRIPCONTROLLER_H
#define TRIPCONTROLLER_H

#include <QObject>
#include <QStringList>
#include <QDate>
#include "controller.h"
#include "trip.h"



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

    virtual QDate getDateValue(EUISource source);
    virtual bool getBooleanValue(EUISource source);
    virtual int getIntValue(EUISource source);

protected:

private:
    Trip* m_trip;

};

#endif // TRIPCONTROLLER_H