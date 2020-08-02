#include "smallview.h"

SmallView::SmallView(QWidget* parent):QGraphicsView(parent)
{
    zoomScale = 1;
    isLeftMouseMoving = false;
    connect(this->horizontalScrollBar(), SIGNAL(sliderReleased()), this, SLOT(onScrollBarReleased()));
    connect(this->verticalScrollBar(), SIGNAL(sliderReleased()), this, SLOT(onScrollBarReleased()));
}

void SmallView::setMyScene(QGraphicsScene *s){
    this->setScene(s);
    centerX = s->sceneRect().width()/2;
    centerY = s->sceneRect().height()/2;
}

void SmallView::keyPressEvent(QKeyEvent *event){
    int moveStep = KEY_MOVE_SPEED/zoomScale;
    if(event->key() == Qt::Key_A){
        centerX -= moveStep;
        centerOn(centerX, centerY);
    }
    else if(event->key() == Qt::Key_D){
        centerX += moveStep;
        centerOn(centerX, centerY);
    }
    else if(event->key() == Qt::Key_W){
        centerY -= moveStep;
        centerOn(centerX, centerY);
    }
    else if(event->key() == Qt::Key_S){
        centerY += moveStep;
        centerOn(centerX, centerY);
    }
}

void SmallView::wheelEvent(QWheelEvent *event){
    int numValue = event->delta();
    double numDegrees = numValue / 8.0;
    double numSteps = numDegrees / 15.0;
    double factor = 1 + numSteps * WHELL_SCALE_SPEED;
    if(zoomScale * factor > 0.05 && zoomScale * factor < 4){
        zoomScale *= factor;
        scale(factor, factor);
        centerOn(centerX, centerY);
    }
}

void SmallView::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        startPoint = event->pos();
        isLeftMouseMoving = true;
    }
    else if(event->button() == Qt::RightButton){
        QPointF scenePoint = this->mapToScene(event->pos());
        qDebug() << scenePoint;
        interactiveScenePoint(scenePoint);
    }
}

void SmallView::mouseMoveEvent(QMouseEvent *event){
    if(isLeftMouseMoving){
        QPointF disPoint = event->pos() - startPoint;
        disPoint /= zoomScale;
        this->centerOn(centerX-disPoint.x(), centerY-disPoint.y());
    }
}

void SmallView::mouseReleaseEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        if(isLeftMouseMoving){
            QPointF disPoint = event->pos() - startPoint;
            disPoint /= zoomScale;
            this->centerOn(centerX-disPoint.x(), centerY-disPoint.y());
            centerX = centerX-disPoint.x();
            centerY = centerY-disPoint.y();
            isLeftMouseMoving = false;
        }
    }
}

void SmallView::onScrollBarReleased(){
    int W = this->width() - this->verticalScrollBar()->width();
    int H = this->height() - this->horizontalScrollBar()->height();
    QPoint p(W/2, H/2);
    QPointF sp = this->mapToScene(p);
    centerX = sp.x();
    centerY = sp.y();
    centerOn(centerX, centerY);
}
