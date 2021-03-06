#ifndef STATISTICSCONTROLLER_H
#define STATISTICSCONTROLLER_H
#include <QMap>
#include "controller.h"
#include "trollingstatisticstable.h"

class TrollingStatistics;

class StatisticsController : public Controller
{
    Q_OBJECT
public:
    explicit StatisticsController(QObject *parent = 0);
    virtual void textEvent(EUISource, const QString&);
    virtual bool getBooleanValue(EUISource);
    QStringList getFields();
    QStringList getNumericFields();
    QStringList getEngines();
    QStringList getOperators();
    QStringList getComparisonOperators();
    TrollingStatisticsTable getStats3D();

signals:
    void progress(int);

private:
    TrollingStatistics* m_stats;
    QString m_filterField;
};

#endif // STATISTICSCONTROLLER_H
