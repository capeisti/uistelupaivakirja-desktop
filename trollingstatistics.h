#ifndef TROLLINGSTATISTICS_H
#define TROLLINGSTATISTICS_H
#include <QObject>
#include <QMap>
#include "singletons.h"

class TrollingStatistics: public QObject
{
    Q_OBJECT
public:
    enum EUnit { eCount, eFishPerTime, eMean, eSum };
    TrollingStatistics();

    virtual QMap<QString, QString> stats() = 0;
    void setY(const QString&);
    void setUnitField(const QString&);
    void setUnit(EUnit);
    void setScaling(bool);

protected:
    QMap<QString, QString> calculate(const QList<QMap<QString, QString> >&);


private:
    QMap<QString, double> countFields(const QList<QMap<QString, QString> >&, const QString&);
    QMap<QString, double> sumFields(const QList<QMap<QString, QString> >&, const QString&);
    QString makeGroup(const QString&);
    QString m_Y;
    QString m_field;
    EUnit m_unit;
    bool m_doScaling;
};

#endif // TROLLINGSTATISTICS_H