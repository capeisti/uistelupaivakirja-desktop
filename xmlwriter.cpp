#include <QDebug>
#include <QFile>
#include <QDomDocument>
#include <QDomElement>
#include <QDomAttr>
#include "xmlwriter.h"

XMLWriter::XMLWriter(const QString& p_filename):
        m_filename(p_filename)
{

}

bool XMLWriter::write(TrollingObject* p_object)
{
    if(!loadDocument())
        return false;

    QDomElement trollingObject;
    int id = p_object->getId();

    if(!getTrollingObjectElement(trollingObject, id))
        return false;

    p_object->setId(id);
    clearNodeContents(trollingObject);

    trollingObject.setAttribute("type", p_object->getType());

    QMapIterator<QString, QVariant> iter(p_object->getProperties());
    while( iter.hasNext() )
    {
        iter.next();
        QDomElement property = m_document.createElement(iter.key());
        qDebug() << "Write property:"<<iter.key() << "value:"<<iter.value().toString();

        QDomText text = m_document.createTextNode(iter.value().toString());
        property.appendChild(text);
        trollingObject.appendChild(property);
    }

    QFile file(m_filename);
    if(!file.open(QIODevice::WriteOnly))
        return false;

    file.write(m_document.toByteArray());
    file.close();
    return true;
}

void XMLWriter::clearNodeContents(QDomNode& p_node)
{
    while( p_node.hasChildNodes() )
    {
        QDomNode node = p_node.childNodes().at(0);
        p_node.removeChild(node);
        node.clear();
    }
}

bool XMLWriter::load(TrollingObject* p_object, int p_id)
{
    if(!loadDocument())
        return false;

    QDomElement trollingObject;
    int id = p_id;
    if(!getTrollingObjectElement(trollingObject, id) || p_id != id)
    {
        qDebug() << "Cant load object from XML!";
        return false;
    }

    QString type = trollingObject.attribute("type");
    if(p_object->getType() != type)
    {
        qCritical() << "Type does not match with loaded object";
        return false;
    }

    QDomNodeList propertiesnodes = trollingObject.childNodes();
    QMap<QString, QVariant> properties;
    for(int loop=0; loop < propertiesnodes.size(); loop++)
    {
        QDomNode node = propertiesnodes.at(loop);
        if(node.isElement())
        {
            QDomElement element = node.toElement();
            qDebug() << "Read property:" << element.tagName() << "value:" << element.text();
            properties[element.tagName()] = element.text();
        }
    }
    p_object->storeProperties(properties);
    p_object->setId(id);
    return true;
}

bool XMLWriter::getTrollingObjectElement(QDomElement& p_element, int& p_id)
{
    QDomElement root;
    int maxId=1;
    if(m_document.elementsByTagName("TrollingObjects").count() == 0) //document is empty
    {
        root = m_document.createElement("TrollingObjects");
        root.setAttribute("MaxId", QString::number(maxId));
        m_document.appendChild(root);
    }
    else if(m_document.elementsByTagName("TrollingObjects").count() == 1) //document is OK
    {
        QDomNode node = m_document.elementsByTagName("TrollingObjects").at(0);
        if(node.isElement())
        {
            root = node.toElement();
            maxId = root.attribute("MaxId").toInt();
        }
        else
        {
            qDebug() << "TrollingObject is not element";
            return false;
        }
    }
    else //invalid document. Cant read.
    {
        qDebug() << "Invalid count of trollingobjects nodes";
        return false;
    }

    QDomNodeList objects = root.elementsByTagName("TrollingObject");

    for(int loop=0; loop < objects.size(); loop++)
    {
        QDomNode node = objects.at(loop);
        if(node.isElement())
        {
            QDomElement element = node.toElement();
            QString id = element.attribute("id");
            if(id.toInt() == p_id)
            {
                p_element = element;
                return true;
            }
        }
    }

    QDomElement element = m_document.createElement("TrollingObject");
    maxId += 1;
    element.setAttribute("id", QString::number(maxId));
    root.appendChild(element);
    root.setAttribute("MaxId", QString::number(maxId));
    p_element = element;
    p_id = maxId;
    return true;
}

QList<int> XMLWriter::getIds(const QString& p_type)
{
    QList<int> ids;
    if(!loadDocument())
        return ids;

    QDomNodeList nodelist = m_document.elementsByTagName("TrollingObject");
    for(int loop=0; loop < nodelist.size(); loop++)
    {
        QDomNode trollingnode = nodelist.at(loop);
        if(trollingnode.isElement())
        {
            QDomElement element = trollingnode.toElement();
            if(p_type.isNull() || element.attribute("type") == p_type)
            {
                ids.append(element.attribute("id").toInt());
            }
        }
    }
    return ids;
}

bool XMLWriter::loadDocument()
{
    QFile file(m_filename);
    if(file.open(QIODevice::ReadOnly))
    {
        QString error;
        int errorLine = 0;
        int errorColumn = 0;

        if(!m_document.setContent(&file, &error, &errorLine, &errorColumn))
        {
            qDebug() << "Cant' read from document" << error << errorLine << " " << errorColumn;
            file.close();
            return false;
        }
    }
    file.close();
    return true;
}