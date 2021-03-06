#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QWheelEvent>
#include "myscene.h"
#include <QMouseEvent>
#include <QFileDialog>
#include <QDebug>
#include "previewwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    //void keyPressEvent(QKeyEvent* event);
    //void wheelEvent(QWheelEvent* event);
    //void mousePressEvent(QMouseEvent* event);
    //void mouseMoveEvent(QMouseEvent* event);

private slots:
    void on_checkBoxShowGrid_stateChanged(int arg1);

    void on_actionexit_triggered();

    void on_radioButtonEdit_toggled(bool checked);

    void on_radioButtonMove_toggled(bool checked);

    void setRadioButtonMove();
    void setRadioButtonEdit();


    void on_pushButtonStart_clicked();

    void on_pushButtonStop_clicked();

    void on_radioButtonPower_toggled(bool checked);

    void on_radioButtonHLine_toggled(bool checked);

    void on_radioButtonVLine_toggled(bool checked);

    void on_pushButtonReset_clicked();

    void on_radioButtonLULine_toggled(bool checked);

    void on_radioButtonLDLine_toggled(bool checked);

    void on_radioButtonRULine_toggled(bool checked);

    void on_radioButtonRDLine_toggled(bool checked);

    void on_radioButtonLight_toggled(bool checked);

    void on_checkBoxNight_stateChanged(int arg1);

    void on_radioButtonCrossConnected_toggled(bool checked);

    void on_checkBoxRenderLight_stateChanged(int arg1);

    void on_radioButtonCrossNotConnected_toggled(bool checked);

    void on_radioButtonSwitch_toggled(bool checked);

    void on_radioButtonAndGate_toggled(bool checked);

    void on_radioButtonOrGate_toggled(bool checked);

    void on_radioButtonNotGate_toggled(bool checked);

    void on_radioButtonXorGate_toggled(bool checked);

    void on_radioButtonSelect_toggled(bool checked);

    void on_radioButtonPaste_toggled(bool checked);

    void on_radioButtonSetPoint_toggled(bool checked);

    void on_pushButtonDelete_clicked();

    void on_radioButtonSelectMove_toggled(bool checked);


    void on_pushButtonClearSelect_clicked();

    void on_pushButtonSaveFile_clicked();

    void on_radioButtonLoadFile_toggled(bool checked);

    void on_radioButtonWall_toggled(bool checked);

    void on_radioButtonLatch_toggled(bool checked);

    void on_spinBoxLightDis_valueChanged(int arg1);

    void on_radioButtonOneWayUp_toggled(bool checked);

    void on_radioButtonOneWayDown_toggled(bool checked);

    void on_radioButtonOneWayLeft_toggled(bool checked);

    void on_radioButtonOneWayRight_toggled(bool checked);

    void on_pushButtonShowPreview_clicked();

    void on_spinBoxSceneW_valueChanged(int arg1);

    void on_spinBoxSceneH_valueChanged(int arg1);

private:
    Ui::MainWindow *ui;
    MyScene *scene1;

    //double graphicsViewZoomScale;
    //int centerOnX;
    //int centerOnY;

    QPointF startPoint;
};
#endif // MAINWINDOW_H
