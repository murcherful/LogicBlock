#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "common.h"
#include "myscene.h"
#include <QGraphicsView>
#include <QGraphicsRectItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->graphicsView, &MyGraphicsView::graphicsEdit, this, &MainWindow::setRadioButtonEdit);
    connect(ui->graphicsView, &MyGraphicsView::graphicsMove, this, &MainWindow::setRadioButtonMove);
    scene1 = new MyScene();
    scene1->setSceneRect(0, 0, SCENE_WIDTH, SCENE_HEIGHT);
    ui->graphicsView->setScene(scene1);
    ui->graphicsView->setMyScene(scene1);
    ui->graphicsView->show();
    ui->labelState->setText("Stopping");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_checkBoxShowGrid_stateChanged(int arg1)
{
    if(arg1 == Qt::Checked){
        scene1->setIsDrawGrid(true);
    }
    else{
        scene1->setIsDrawGrid(false);
    }
    scene1->update();
}

void MainWindow::on_actionexit_triggered()
{
    this->close();
}

void MainWindow::on_radioButtonEdit_toggled(bool checked)
{
    if(checked){
        ui->graphicsView->setGraphicsState(MyGraphicsView::STATE_EDIT);
    }
}

void MainWindow::on_radioButtonMove_toggled(bool checked)
{
    if(checked){
        ui->graphicsView->setGraphicsState(MyGraphicsView::STATE_MOVE);
    }
}

void MainWindow::setRadioButtonMove(){
    ui->radioButtonMove->setChecked(true);
}

void MainWindow::setRadioButtonEdit(){
    ui->radioButtonEdit->setChecked(true);
}


void MainWindow::on_pushButtonStart_clicked()
{
    int breakTime = ui->spinBoxBreakTime->value();
    ui->graphicsView->startTimer(breakTime);
    ui->labelState->setText("Running");
}

void MainWindow::on_pushButtonStop_clicked()
{
    ui->graphicsView->stopTimer();
    ui->labelState->setText("Stopping");
}

void MainWindow::on_radioButtonPower_toggled(bool checked)
{
    if(checked){
        ui->graphicsView->setBlockType(BasicBlock::POWER);
    }
}

void MainWindow::on_radioButtonHLine_toggled(bool checked)
{
    if(checked){
        ui->graphicsView->setBlockType(BasicBlock::STRAIGHT_LINE_H);
    }
}

void MainWindow::on_radioButtonVLine_toggled(bool checked)
{
    if(checked){
        ui->graphicsView->setBlockType(BasicBlock::STRAIGHT_LINE_V);
    }
}

void MainWindow::on_pushButtonReset_clicked()
{
    ui->graphicsView->resetBlock();
}

void MainWindow::on_radioButtonLULine_toggled(bool checked)
{
    if(checked){
        ui->graphicsView->setBlockType(BasicBlock::LEFT_UP_LINE);
    }
}

void MainWindow::on_radioButtonLDLine_toggled(bool checked)
{
    if(checked){
        ui->graphicsView->setBlockType(BasicBlock::LEFT_DOWN_LINE);
    }
}

void MainWindow::on_radioButtonRULine_toggled(bool checked)
{
    if(checked){
        ui->graphicsView->setBlockType(BasicBlock::RIGHT_UP_LINE);
    }
}

void MainWindow::on_radioButtonRDLine_toggled(bool checked)
{
    if(checked){
        ui->graphicsView->setBlockType(BasicBlock::RIGHT_DOWN_LINE);
    }
}

void MainWindow::on_radioButtonLight_toggled(bool checked)
{
    if(checked){
        ui->graphicsView->setBlockType(BasicBlock::LIGHT);
    }
}

void MainWindow::on_checkBoxNight_stateChanged(int arg1)
{
    if(arg1 == Qt::Checked){
        scene1->setIsNight(true);
    }
    else{
        scene1->setIsNight(false);
    }
    scene1->update();
}

void MainWindow::on_radioButtonCrossConnected_toggled(bool checked)
{
    if(checked){
        ui->graphicsView->setBlockType(BasicBlock::CROSS_CONNECT);
    }
}

void MainWindow::on_checkBoxRenderLight_stateChanged(int arg1)
{
    if(arg1 == Qt::Checked){
        scene1->setIsRenderLight(true);
    }
    else{
        scene1->setIsRenderLight(false);
    }
    scene1->update();
}

void MainWindow::on_radioButtonCrossNotConnected_toggled(bool checked)
{
    if(checked){
        ui->graphicsView->setBlockType(BasicBlock::CROSS_NOT_CONNECT);
    }
}

void MainWindow::on_radioButtonSwitch_toggled(bool checked)
{
    if(checked){
        ui->graphicsView->setBlockType(BasicBlock::SWITCH_BLOCK);
    }
}

void MainWindow::on_radioButtonAndGate_toggled(bool checked)
{
    if(checked){
        ui->graphicsView->setBlockType(BasicBlock::AND_GATE_BLOCK);
    }
}

void MainWindow::on_radioButtonOrGate_toggled(bool checked)
{
    if(checked){
        ui->graphicsView->setBlockType(BasicBlock::OR_GATE_BLOCK);
    }
}

void MainWindow::on_radioButtonNotGate_toggled(bool checked)
{
    if(checked){
        ui->graphicsView->setBlockType(BasicBlock::NOT_GATE_BLOCK);
    }
}

void MainWindow::on_radioButtonXorGate_toggled(bool checked)
{
    if(checked){
        ui->graphicsView->setBlockType(BasicBlock::XOR_GATE_BLOCK);
    }
}

void MainWindow::on_radioButtonSelect_toggled(bool checked)
{
    if(checked){
        ui->graphicsView->setGraphicsState(MyGraphicsView::STATE_SELECT);
    }
}

void MainWindow::on_radioButtonPaste_toggled(bool checked)
{
    if(checked){
        ui->graphicsView->setSelectState(MyGraphicsView::SELECT_STATE_PASTE);
    }
}

void MainWindow::on_radioButtonSetPoint_toggled(bool checked)
{
    if(checked){
        ui->graphicsView->setSelectState(MyGraphicsView::SELECT_STATE_SET_POINT);
    }
}

void MainWindow::on_pushButtonDelete_clicked()
{
    ui->graphicsView->deleteSelectArea();
}

void MainWindow::on_radioButtonSelectMove_toggled(bool checked)
{
    if(checked){
        ui->graphicsView->setSelectState(MyGraphicsView::SELECT_STATE_MOVE);
    }
}

void MainWindow::on_pushButtonClearSelect_clicked()
{
    ui->graphicsView->clearSelectPoint();
}

void MainWindow::on_pushButtonSaveFile_clicked()
{
    ui->graphicsView->saveSelectArea();
}

void MainWindow::on_radioButtonLoadFile_toggled(bool checked)
{
    if(checked){
        ui->graphicsView->setSelectState(MyGraphicsView::SELECT_LOAD_FILE);
    }
}
