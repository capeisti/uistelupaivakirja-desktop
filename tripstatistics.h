#ifndef TRIPSTATISTICS_H
#define TRIPSTATISTICS_H
#include "trollingstatistics.h"

class TripStatistics : public TrollingStatistics
{
    Q_OBJECT
public:
    explicit TripStatistics(QObject *parent = 0);    
    virtual QStringList getTextFields();
    virtual QStringList getNumericFields();
    virtual QString getName();
    virtual QStringList getOperators();

protected:
    virtual QHash<QString, QString> stats();
    virtual QHash<QString, QString> calculate(const QList<QHash<QString, QString> >&);

signals:

public slots:

};

#endif // TRIPSTATISTICS_H
