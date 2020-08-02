#ifndef SMALLVIEW_H
#define SMALLVIEW_H
#include <QGraphicsView>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QMouseEvent>
#include "common.h"
#include <QDebug>
#include <QScrollBar>

class SmallView:public QGraphicsView
{
    Q_OBJECT

public:
    SmallView(QWidget* parent=nullptr);
    void setMyScene(QGraphicsScene* s);

protected:
    void keyPressEvent(QKeyEvent* event);
    void wheelEvent(QWheelEvent* event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent* event);
signals:
    void interactiveScenePoint(QPointF scenePoint);
private slots:
    void onScrollBarReleased();

private:
    double zoomScale;
    int centerX;
    int centerY;
    bool isLeftMouseMoving;

    QPointF startPoint;

};

#endif // SMALLVIEW_H
