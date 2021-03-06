#include "lureform.h"
#include "ui_lureform.h"

LureForm::LureForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LureForm)
{
    ui->setupUi(this);
    m_lureController = Singletons::lureController();
    ui->lure_list->sortByColumn(0, Qt::AscendingOrder);

    observerEvent(Controller::eLureListUpdated);

    connect(m_lureController, SIGNAL(observerNotification(int)), this, SLOT(observerEvent(int)));
    connect(Singletons::syncController(), SIGNAL(observerNotification(int)), this, SLOT(observerEvent(int)));
}

LureForm::~LureForm()
{
    disconnect(m_lureController, SIGNAL(observerNotification(int)), this, SLOT(observerEvent(int)));
    disconnect(Singletons::syncController(), SIGNAL(observerNotification(int)), this, SLOT(observerEvent(int)));
    delete ui;
}



void LureForm::observerEvent(int type)
{
    ui->lure_save->setDisabled(!m_lureController->getBooleanValue(eUnsavedChanges));
    ui->lure_undo->setDisabled(!m_lureController->getBooleanValue(eUnsavedChanges));
    ui->lure_delete->setDisabled(!m_lureController->getBooleanValue(eLureList));

    ui->lure_manufacturer->setDisabled(!m_lureController->getBooleanValue(eLureList));
    ui->lure_model->setDisabled(!m_lureController->getBooleanValue(eLureList));
    ui->lure_size->setDisabled(!m_lureController->getBooleanValue(eLureList));
    ui->lure_type->setDisabled(!m_lureController->getBooleanValue(eLureList));
    ui->lure_color->setDisabled(!m_lureController->getBooleanValue(eLureList));
    ui->lure_favorite->setDisabled(!m_lureController->getBooleanValue(eLureList));
    ui->lure_notvisible->setDisabled(!m_lureController->getBooleanValue(eLureList));
    ui->lure_nickname->setDisabled(!m_lureController->getBooleanValue(eLureList));
    ui->lure_back->setDisabled(!m_lureController->getBooleanValue(eLureList));
    ui->lure_side->setDisabled(!m_lureController->getBooleanValue(eLureList));
    ui->lure_belly->setDisabled(!m_lureController->getBooleanValue(eLureList));
    ui->lure_class->setDisabled(!m_lureController->getBooleanValue(eLureList));
    ui->lure_contrast->setDisabled(!m_lureController->getBooleanValue(eLureList));

    if(type == Controller::eLureListUpdated)
    {
        ui->lure_list->blockSignals(true);
        ui->lure_list->setSortingEnabled(false);
        ui->lure_list->clear();
        ui->lure_list->clearContents();
        ui->lure_list->setRowCount(0);
        ui->lure_list->setColumnCount(m_lureController->getLureColumns().count());
        ui->lure_list->setHorizontalHeaderLabels(m_lureController->getLureColumns());

        int selectedLure = m_lureController->getIntValue(eLureList);
        QList<QList<QString> > lures = m_lureController->getLureListLong();
        for(int loop=0; loop < lures.size(); loop++)
        {
            QList<QString> lure = lures.at(loop);
            ui->lure_list->insertRow(loop);
            QString id = lure.at(lure.count()-1);

            for(int col = 0; col < lure.count()-1; col++)
            {
                QTableWidgetItem* item = new QTableWidgetItem(lure.at(col));
                item->setData(Qt::UserRole+1, id);
                ui->lure_list->setItem(loop, col, item);
            }
            if(id.toInt() == selectedLure)
            {
                ui->lure_list->selectRow(loop);
            }
        }
        ui->lure_list->setSortingEnabled(true);
        ui->lure_list->blockSignals(false);
    } else if(type == Controller::eLureUpdated)
    {
        setCombo(eLureMaker, ui->lure_manufacturer);
        setCombo(eLureModel, ui->lure_model);
        setCombo(eLureSize, ui->lure_size);
        setCombo(eLureColor, ui->lure_color);
        setCombo(eLureType, ui->lure_type);
        setCombo(eLureColorBack, ui->lure_back);
        setCombo(eLureColorSide, ui->lure_side);
        setCombo(eLureColorBelly, ui->lure_belly);
        //setCombo(eLureColorClass, ui->lure_class);
        setCombo(eLureColorContrast, ui->lure_contrast);

        QString colorclass = m_lureController->getTextValue(eLureColorClass);
        int colorindex = ui->lure_class->findText(colorclass);
        ui->lure_class->blockSignals(true);
        ui->lure_class->setCurrentIndex(colorindex);
        ui->lure_class->blockSignals(false);

        ui->lure_favorite->setChecked(m_lureController->getBooleanValue(eLureFavorite));
        ui->lure_notvisible->setChecked(m_lureController->getBooleanValue(eLureNotVisible));
        ui->lure_nickname->blockSignals(true);
        ui->lure_nickname->setText(m_lureController->getTextValue(eLureNickName));
        ui->lure_nickname->blockSignals(false);
    }
}

void LureForm::setCombo(EUISource source, QComboBox* target)
{
    QStringList valuelist = m_lureController->getAlternatives(source);
    QString currentValue = m_lureController->getTextValue(source);
    target->blockSignals(true);
    target->clear();
    foreach(QString value, valuelist)
    {
        if(target->findText(value) == -1)
        {
            target->addItem(value);
        }
    }
    target->setEditText(currentValue);
    target->blockSignals(false);
}

void LureForm::on_lure_new_clicked()
{    
    m_lureController->buttonEvent(eLureNew);
    ui->lure_manufacturer->setFocus();
}

void LureForm::on_lure_delete_clicked()
{
    m_lureController->buttonEvent(eLureDelete);
}

void LureForm::on_lure_favorite_clicked(bool checked)
{
    m_lureController->booleanEvent(eLureFavorite, checked);
}

void LureForm::on_lure_list_itemSelectionChanged()
{
    QList<QTableWidgetItem*> selected = ui->lure_list->selectedItems();
    if(selected.size() >= 1)
        m_lureController->intEvent(eLureList, selected.at(0)->data(Qt::UserRole+1).toInt());
}

void LureForm::on_lure_manufacturer_textChanged(QString text)
{
    m_lureController->textEvent(eLureMaker, text);
}

void LureForm::on_lure_model_textChanged(QString text)
{
   m_lureController->textEvent(eLureModel, text);
}

void LureForm::on_lure_size_textChanged(QString text)
{
    m_lureController->textEvent(eLureSize, text);
}

void LureForm::on_lure_color_textChanged(QString text)
{
   m_lureController->textEvent(eLureColor, text);
}

void LureForm::on_lure_type_textChanged(QString text)
{
    m_lureController->textEvent(eLureType, text);
}

void LureForm::on_lure_undo_clicked()
{
    m_lureController->buttonEvent(eLureUndo);
}

void LureForm::on_lure_save_clicked()
{
    m_lureController->buttonEvent(eLureSave);
}

void LureForm::on_lure_notvisible_clicked(bool checked)
{
    m_lureController->booleanEvent(eLureNotVisible, checked);
}

void LureForm::on_lure_nickname_textChanged(QString text)
{
   m_lureController->textEvent(eLureNickName, text);
}

void LureForm::on_lure_back_textChanged(QString text)
{
    m_lureController->textEvent(eLureColorBack, text);
}

void LureForm::on_lure_side_textChanged(QString text)
{
    m_lureController->textEvent(eLureColorSide, text);
}

void LureForm::on_lure_belly_textChanged(QString text)
{
    m_lureController->textEvent(eLureColorBelly, text);
}

void LureForm::on_lure_class_textChanged(QString text)
{

}

void LureForm::on_lure_contrast_textChanged(QString text)
{
    m_lureController->textEvent(eLureColorContrast, text);
}

void LureForm::on_lure_class_currentIndexChanged(QString text)
{
    m_lureController->textEvent(eLureColorClass, text);
}
