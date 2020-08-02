#ifndef PREVIEWWINDOW_H
#define PREVIEWWINDOW_H

#include <QWidget>
#include <QGraphicsView>
#include "mygraphicsview.h"

namespace Ui {
class PreviewWindow;
}

class PreviewWindow : public QWidget
{
    Q_OBJECT

public:
    explicit PreviewWindow(QWidget *parent = nullptr);
    ~PreviewWindow();

    void setScene(MyGraphicsView* mgv);

private:
    Ui::PreviewWindow *ui;
};

#endif // PREVIEWWINDOW_H
