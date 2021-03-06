#ifndef PLACEFORM_H
#define PLACEFORM_H

#include <QWidget>
#include <QTableWidgetItem>
#include "singletons.h"

namespace Ui {
    class PlaceForm;
}

class PlaceForm : public QWidget
{
    Q_OBJECT

public:
    explicit PlaceForm(QWidget *parent = 0);
    ~PlaceForm();

private slots:
    void on_place_save_clicked();
    void on_place_undo_clicked();
    void observerEvent(int type);

    void on_place_list_itemSelectionChanged();
    void on_place_delete_clicked();
    void on_place_new_clicked();
    void on_place_misc_textChanged();
    void on_place_invisible_clicked(bool checked);
    void on_place_city_textEdited(QString );
    void on_place_name_textEdited(QString );

private:
    Ui::PlaceForm *ui;
    PlaceController* m_placeController;
};

#endif // PLACEFORM_H
