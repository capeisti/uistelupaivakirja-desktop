#include "uieventhandler.h"
#include "mainwindow.h"
//#include "ui_mainwindow.h"

UIEventHandler::UIEventHandler(MainWindow* p_mw, QObject *parent) :
    QObject(parent),
    mw(p_mw)
{
    ui = mw->ui;
    m_tripController = Singletons::tripController();
    m_lureController = Singletons::lureController();
    m_placeController = Singletons::placeController();
    observerEvent(Controller::eTripUpdated);
    observerEvent(Controller::eTripListUpdated);
    observerEvent(Controller::eFishListUpdated);
    observerEventLure(Controller::eLureListUpdated);
    observerEventPlace(Controller::ePlaceListUpdated);

    connect(m_tripController, SIGNAL(observerNotification(int)), this, SLOT(observerEvent(int)));
    connect(m_lureController, SIGNAL(observerNotification(int)), this, SLOT(observerEventLure(int)));
    connect(m_placeController, SIGNAL(observerNotification(int)), this, SLOT(observerEventPlace(int)));
}

UIEventHandler::~UIEventHandler()
{
    disconnect(m_tripController, SIGNAL(observerNotification(int)), this, SLOT(observerEvent(int)));
    disconnect(m_lureController, SIGNAL(observerNotification(int)), this, SLOT(observerEventLure(int)));
    disconnect(m_placeController, SIGNAL(observerNotification(int)), this, SLOT(observerEventPlace(int)));
}

void UIEventHandler::observerEvent(int type)
{
    if(type == Controller::eTripUpdated)
    {
         ui->dateEdit->setDate(m_tripController->getDateValue(eTripDate));

         ui->startDial->setValue(m_tripController->getIntValue(eStartTime));
         ui->timeStartLbl->setText(tr("aloitus klo ")+QString::number(m_tripController->getIntValue(eStartTime)));

         ui->endDial->setValue(m_tripController->getIntValue(eEndTime));
         ui->timeEndLbl->setText(tr("lopetus klo ")+QString::number(m_tripController->getIntValue(eEndTime)));

         ui->weather_clear->setChecked(m_tripController->getBooleanValue(eWeatherClear));
         ui->weather_halfclear->setChecked(m_tripController->getBooleanValue(eWeatherHalfClear));
         ui->weather_overcast->setChecked(m_tripController->getBooleanValue(eWeatherOvercast));
         ui->weather_rain->setChecked(m_tripController->getBooleanValue(eWeatherRain));
         ui->weather_fog->setChecked(m_tripController->getBooleanValue(eWeatherFog));

         ui->wind_calm->setChecked(m_tripController->getBooleanValue(eWindCalm));
         ui->wind_faint->setChecked(m_tripController->getBooleanValue(eWindFaint));
         ui->wind_moderate->setChecked(m_tripController->getBooleanValue(eWindModerate));
         ui->wind_brisk->setChecked(m_tripController->getBooleanValue(eWindBrisk));
         ui->wind_hard->setChecked(m_tripController->getBooleanValue(eWindHard));

         ui->pressure_low->setChecked(m_tripController->getBooleanValue(ePressureLow));
         ui->pressure_mildlow->setChecked(m_tripController->getBooleanValue(ePressureMildLow));
         ui->pressure_normal->setChecked(m_tripController->getBooleanValue(ePressureNormal));
         ui->pressure_mildhigh->setChecked(m_tripController->getBooleanValue(ePressureMildHigh));
         ui->pressure_high->setChecked(m_tripController->getBooleanValue(ePressureHigh));

         ui->length->setText(m_tripController->getTextValue(eLength));
         ui->weight->setText(m_tripController->getTextValue(eWeight));
         ui->spotdepth->setText(m_tripController->getTextValue(eSpotDepth));
         ui->totaldepth->setText(m_tripController->getTextValue(eTotalDepth));
         ui->trolling_speed->setText(m_tripController->getTextValue(eTrollingSpeed));
         ui->line_weight->setText(m_tripController->getTextValue(eLineWeight));
         ui->release_width->setText(m_tripController->getTextValue(eReleaseWidth));
         ui->air_temp->setText(m_tripController->getTextValue(eAirTemp));
         ui->water_temp->setText(m_tripController->getTextValue(eWaterTemp));

         ui->group->setChecked(m_tripController->getBooleanValue(eGroup));
         ui->undersize->setChecked(m_tripController->getBooleanValue(eUnderSize));
         ui->catchrelease->setChecked(m_tripController->getBooleanValue(eCatchNRelease));

         ui->wind_direction->setValue(m_tripController->getIntValue(eWindDirection));
         ui->pressure_change->setValue(m_tripController->getIntValue(ePressureChange));

         ui->timeEdit->setTime(m_tripController->getTimeValue(eTime));

        if( ui->misc->toPlainText() != m_tripController->getTextValue(eMiscText))
             ui->misc->setText(m_tripController->getTextValue(eMiscText));


         mw->m_lureBox->setText(m_tripController->getTextValue(eLureName));
         mw->m_POIBox->setText(m_tripController->getTextValue(eWayPointSet));

        setCombo(eSpecies,  ui->species);
        setCombo(eGetter,  ui->getter);
        setCombo(eMethod,  ui->method);

        int selectedPlace = m_tripController->getIntValue(ePlaceName);
        for(int loop=0; loop <  ui->place->count(); loop++)
        {
            if( ui->place->itemData(loop).toInt() == selectedPlace)
            {
                 ui->place->setCurrentIndex(loop);
            }
        }

    }
    else if(type == Controller::eTripListUpdated)
    {
         ui->trip_list->clear();
        QMap<QString, int> trips = m_tripController->getTripList();
        for(QMap<QString, int>::iterator iter = trips.begin(); iter != trips.end(); iter++)
        {
            QListWidgetItem* item = new QListWidgetItem(iter.key(),  ui->trip_list, iter.value());
             ui->trip_list->insertItem(0, item);
        }
    }
    else if (type == Controller::eFishListUpdated)
    {
         ui->fish_list->clear();
         ui->fish_list->clearContents();
         ui->fish_list->setRowCount(0);
        QStringList headers;
        headers << tr("aika") <<
                tr("laji") <<
                tr("tapa") <<
                tr("saaja") <<
                tr("viehe") <<
                tr("paino") <<
                tr("pituus") <<
                tr("syvyys") <<
                tr("ved.syv") <<
                tr("vetonop") <<
                tr("vap.pit");
         ui->fish_list->setHorizontalHeaderLabels(headers);
        QList<QMap<QString, QString> > fishes = m_tripController->getFishList();
        for(int loop=0; loop < fishes.size(); loop++)
        {
            QMap<QString, QString> props = fishes.at(loop);
             ui->fish_list->insertRow(loop);
             ui->fish_list->setItem(loop, 0, new QTableWidgetItem(format(props[FISH_TIME]), loop));
             ui->fish_list->setItem(loop, 1, new QTableWidgetItem(format(props[FISH_SPECIES]), loop));
             ui->fish_list->setItem(loop, 2, new QTableWidgetItem(props[FISH_METHOD], loop));
             ui->fish_list->setItem(loop, 3, new QTableWidgetItem(props[FISH_GETTER], loop));
             ui->fish_list->setItem(loop, 4, new QTableWidgetItem(props["lure"], loop));
             ui->fish_list->setItem(loop, 5, new QTableWidgetItem(props[FISH_WEIGHT], loop));
             ui->fish_list->setItem(loop, 6, new QTableWidgetItem(props[FISH_LENGTH], loop));
             ui->fish_list->setItem(loop, 7, new QTableWidgetItem(props[FISH_SPOT_DEPTH], loop));
             ui->fish_list->setItem(loop, 8, new QTableWidgetItem(props[FISH_TOTAL_DEPTH], loop));
             ui->fish_list->setItem(loop, 9, new QTableWidgetItem(props[FISH_TROLLING_SPEED], loop));
             ui->fish_list->setItem(loop, 10, new QTableWidgetItem(props[FISH_RELEASE_WIDTH], loop));
        }
    }
    else if(type == Controller::eWayPointsUpdated)
    {
        mw->m_wptList->clear();
        QList<QPair<QString, int> > list = m_tripController->getWayPointsList();
        for(int loop=0; loop < list.count(); loop++)
        {
            QPair<QString, int> pair = list.at(loop);
            QListWidgetItem* item = new QListWidgetItem(pair.first, mw->m_wptList);
            item->setData(Qt::UserRole+2,  pair.second);
            mw->m_wptList->insertItem(0, item);
        }
    }
}

void UIEventHandler::setCombo(EUISource source, QComboBox* target)
{
    return;
    QStringList valuelist = m_tripController->getAlternatives(source);
    QString currentValue = m_tripController->getTextValue(source);
    foreach(QString value, valuelist)
    {
        if(target->findText(value) == -1)
        {
            target->addItem(value);
        }
    }
    target->setEditText(currentValue);
}

QString UIEventHandler::format(const QString& str)
{
    if(str == "")
        return "n/a";
    return str;
}

void UIEventHandler::observerEventLure(int type)
{
    if(type == Controller::eLureListUpdated)
    {
         ui->lure_list->clear();
         ui->small_lure_list->clear();
        QList<QPair<QString, int> > lures = m_lureController->getLureList();
        for(int loop=0; loop < lures.size(); loop++)
        {
            QPair<QString, int> pair = lures.at(loop);
            QListWidgetItem* item = new QListWidgetItem(pair.first,  ui->lure_list, pair.second);
             ui->lure_list->insertItem(0, item);

            QListWidgetItem* itemSmall = new QListWidgetItem(pair.first,  ui->small_lure_list);
            itemSmall->setData(Qt::UserRole+1,  pair.second);
             ui->small_lure_list->insertItem(0, itemSmall);
        }
    } else if(type == Controller::eLureUpdated)
    {
         ui->lure_manufacturer->setText(m_lureController->getTextValue(eLureMaker));
         ui->lure_model->setText(m_lureController->getTextValue(eLureModel));
         ui->lure_size->setText(m_lureController->getTextValue(eLureSize));
         ui->lure_color->setText(m_lureController->getTextValue(eLureColor));
         ui->lure_favorite->setChecked(m_lureController->getBooleanValue(eLureFavorite));
    }
}

void UIEventHandler::observerEventPlace(int type)
{
    if(type == Controller::ePlaceListUpdated)
    {
         ui->place_list->clear();
         ui->place->clear();
        QList<QPair<QString, int> > places = m_placeController->getPlaceList();
        for(int loop=0; loop < places.size(); loop++)
        {
            QPair<QString, int> pair = places.at(loop);
            QListWidgetItem* item = new QListWidgetItem(pair.first,  ui->place_list, pair.second);
             ui->place_list->insertItem(0, item);

             ui->place->addItem(pair.first, pair.second);
        }
    } else if(type == Controller::ePlaceUpdated)
    {
         ui->place_name->setText( m_placeController->getTextValue(ePlaceName) );
         ui->place_city->setText( m_placeController->getTextValue(ePlaceCity) );
         ui->place_misc->setText( m_placeController->getTextValue(ePlaceMiscText) );
         ui->place_invisible->setChecked(m_placeController->getBooleanValue(ePlaceInvisible));
    }
}