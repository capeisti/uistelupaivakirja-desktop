#include <QDebug>
#include <QTimer>
#include "singletons.h"
#include "synchronizercontroller.h"
#include "revisiondialog.h"



SynchronizerController::SynchronizerController(QObject *parent) :
    Controller(parent)
{
    m_sync = Singletons::model()->getSynchronizer();
    connect(m_sync, SIGNAL(downloadDone()), this, SLOT(downloadDone()));
    connect(m_sync, SIGNAL(uploadDone()), this, SLOT(uploadDone()));
    connect(m_sync, SIGNAL(error(QString)), this, SLOT(error(QString)));
    connect(Singletons::model(), SIGNAL(save()), this, SLOT(uploadChanges()));
}

void SynchronizerController::checkUpdates()
{
    showStatusMessage(tr("Tutkitaan onko palvelimella päivityksiä tietokantaan"), true);
    //m_sync->download();
}

void SynchronizerController::downloadDone()
{
    qDebug() << "got download done";
    QList<TrollingObject*> changesAdded = m_sync->getChanges(Synchronizer::eAdded);
    QList<TrollingObject*> changesModified = m_sync->getChanges(Synchronizer::eModified);
    QList<TrollingObject*> changesRemoved = m_sync->getChanges(Synchronizer::eRemoved);
    QList<TrollingObject*> changesConflicting = m_sync->getChanges(Synchronizer::eConflicting);

    int totalChanges = changesAdded.size() + changesConflicting.size() + changesModified.size() + changesRemoved.size();

    if(totalChanges == 0)
    {
        m_sync->saveChanges(changesAdded);
    }
    else
    {
        RevisionDialog dlg;

        dlg.setAdded(trollingObjectToStringList(changesAdded));
        dlg.setModified(trollingObjectToStringList(changesModified));
        dlg.setConflicting(trollingObjectToStringList(changesConflicting));
        dlg.setRemoved(trollingObjectToStringList(changesRemoved));
        dlg.setModal(true);
        if(dlg.exec() == QDialog::Accepted)
        {
            QList<TrollingObject*> changes;
            if(!dlg.isDontSave())
            {
                changes += changesAdded;
                changes += changesConflicting;
                changes += changesModified;
                changes += changesRemoved;
            }

            m_sync->saveChanges(changes);
            sendNotificationToObservers(Controller::eTripListUpdated);
            sendNotificationToObservers(Controller::eLureListUpdated);
            sendNotificationToObservers(Controller::ePlaceListUpdated);

            sendNotificationToObservers(Controller::eTripUpdated);
            sendNotificationToObservers(Controller::eLureUpdated);
            sendNotificationToObservers(Controller::ePlaceUpdated);
        }
    }

    showStatusMessage(tr("Katsottu päivitykset palvelusta"), false);
}

void SynchronizerController::uploadChanges()
{
    showStatusMessage(tr("Tietokannan synkronointi palvelun kanssa käynnissä"), true);
    m_sync->upload();
}

void SynchronizerController::uploadDone()
{
    showStatusMessage(tr("Tietokanta synkronoitu palvelun kanssa"), false);
    //m_sync->download();
}

void SynchronizerController::error(const QString& error)
{
    showStatusMessage(tr("Palvelun kanssa oli ongelmia. Yritetään uudelleen hetken kuluttua"), false);
}

QStringList SynchronizerController::trollingObjectToStringList(const QList<TrollingObject*>& objects)
{
    QStringList retval;
    foreach(TrollingObject* o, objects)
    {
        QString label = getTypeName(o->getType());
        label += ", ";
        label += o->toString();
        retval.append(label);
    }

    return retval;
}

QString SynchronizerController::getTypeName(const QString& type)
{
    if(type == Trip().getType())
    {
        return tr("reissu");
    }
    else if(type == Place().getType())
    {
        return tr("kalapaikka");
    }
    else if(type == Lure().getType())
    {
        return tr("viehe");
    }

    return type;
}
