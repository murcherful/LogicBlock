#include "previewwindow.h"
#include "ui_previewwindow.h"

PreviewWindow::PreviewWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PreviewWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Preview");
}

PreviewWindow::~PreviewWindow()
{
    delete ui;
}

void PreviewWindow::setScene(MyGraphicsView* mgv){
    ui->graphicsView->setMyScene(mgv->scene());
    mgv->connectSmallView(ui->graphicsView);
}
