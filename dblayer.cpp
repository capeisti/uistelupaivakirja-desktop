#include <QDir>
#include <QDebug>
#include "dblayer.h"
#include "xmlwriter.h"

DBLayer::DBLayer(const QString& p_storePath):
m_storeDir(p_storePath)
{

    if(!m_storeDir.exists("."))
    {
        m_storeDir.mkpath(".");
    }
}

bool DBLayer::storeObject(TrollingObject* p_object)
{
    XMLWriter writer(m_storeDir.absolutePath()+"/object.xml");
    return writer.write(p_object);
}

bool DBLayer::loadObject(int id, TrollingObject* p_object)
{
    XMLWriter writer(m_storeDir.absolutePath()+"/object.xml");
    return writer.load(p_object, id);
}

QList<int> DBLayer::getIds(const QString& p_type)
{
    XMLWriter writer(m_storeDir.absolutePath()+"/object.xml");
    return writer.getIds(p_type);
}