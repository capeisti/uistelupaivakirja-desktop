#include <QDir>
#include "trollingmodel.h"

TrollingModel::TrollingModel(QObject *parent) :
    QObject(parent)
{
    m_DBLayer = new DBLayer(QDir::homePath()+"/uistelu/database");
    QList<int> ids = m_DBLayer->getIds();
    for(int loop=0; loop < ids.size(); loop++)
    {
        qDebug() << "id" << ids[loop];
    }
}

TrollingModel::~TrollingModel()
{
    delete m_DBLayer;
}

Trip* TrollingModel::getTrip(int id)
{
    if(id < 0 )
        return new Trip();

    Trip* trip = new Trip();
    m_DBLayer->loadObject(id, trip);
    return trip;
}

Lure* TrollingModel::getLure(int id)
{
    if(id < 0)
        return new Lure();

    return NULL;
}

Site* TrollingModel::getSite(int id)
{
    if(id < 0)
        return new Site();

    return NULL;
}

Species* TrollingModel::getSpecies(int id)
{
    if(id < 0)
        return new Species();

    return NULL;
}

int TrollingModel::commit(TrollingObject* object)
{   
    if(object == NULL)
       return -2;

    m_DBLayer->storeObject(object);
    return object->getId();
}