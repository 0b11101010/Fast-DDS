/*************************************************************************
 * Copyright (c) 2014 eProsima. All rights reserved.
 *
 * This copy of eProsima RTPS ShapesDemo is licensed to you under the terms described in the
 * EPROSIMARTPS_LIBRARY_LICENSE file included in this distribution.
 *
 *************************************************************************/
#ifndef SUBSCRIBEDIALOG_H
#define SUBSCRIBEDIALOG_H

#include <QDialog>

namespace Ui {
class SubscribeDialog;
}

class ShapesDemo;
class ShapeSubscriber;

class SubscribeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SubscribeDialog(ShapesDemo* psd,QWidget *parent = 0);
    ~SubscribeDialog();

    private slots:

        void on_buttonBox_accepted();

        void on_comboBox_ownership_currentIndexChanged(int index);

        void on_checkBox_contentBasedFilter_toggled(bool checked);

        void on_lineEdit_minX_editingFinished();

        void on_lineEdit_maxX_editingFinished();

        void on_lineEdit_minY_editingFinished();

        void on_lineEdit_maxY_editingFinished();

private:
    Ui::SubscribeDialog *ui;
    ShapesDemo* mp_sd;
};

#endif // SUBSCRIBEDIALOG_H
