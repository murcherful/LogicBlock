#include "myscene.h"
#include "common.h"

MyScene::MyScene(QWidget *parent):QGraphicsScene(parent)
{
    isDrawGrid = true;
    isNight = false;
    isRenderLight = false;
    lightSet = NULL;
}

void MyScene::drawForeground(QPainter *painter, const QRectF &rect){
    if(isNight){
        QColor colorBack(0, 0, 0);
        colorBack.setAlphaF(0.5);
        painter->setBrush(QBrush(colorBack));
        painter->drawRect(rect);
    }
    QColor color(255, 255, 0);
    color.setAlphaF(0.75);
    painter->setBrush(QBrush(color));
    MyHashSet::iterator it;
    if(isRenderLight){
        for(it = lightSet->begin(); it != lightSet->end(); it++){
            painter->drawRect((it->first-4)*BLOCK_SIZE, (it->second-4)*BLOCK_SIZE, BLOCK_SIZE*9, BLOCK_SIZE*9);
        }
    }
}

void MyScene::drawBackground(QPainter *painter, const QRectF &rect){
    if(isDrawGrid){
        drawGrid(painter, rect);
    }
}

void MyScene::drawGrid(QPainter *painter, const QRectF &rect){
    int centerX = rect.center().x();
    int centerY = rect.center().y();
    for(int i = 0; i < rect.right(); i += BLOCK_SIZE){
        painter->drawLine(i, rect.top(), i, rect.bottom());
    }
    for(int i = 0; i > rect.left(); i -= BLOCK_SIZE){
        painter->drawLine(i, rect.top(), i, rect.bottom());
    }
    for(int i = 0; i < rect.bottom(); i += BLOCK_SIZE){
        painter->drawLine(rect.left(), i, rect.right(), i);
    }
    for(int i = 0; i > rect.top(); i -= BLOCK_SIZE){
        painter->drawLine(rect.left(), i, rect.right(), i);
    }
}

void MyScene::setIsDrawGrid(bool s){
    isDrawGrid = s;
}

void MyScene::setIsNight(bool s){
    isNight = s;
}

void MyScene::setIsRenderLight(bool s){
    isRenderLight = s;
}
