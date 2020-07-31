#include "myscene.h"
#include "common.h"

MyScene::MyScene(QWidget *parent):QGraphicsScene(parent)
{
    isDrawGrid = true;
    isNight = false;
    isRenderLight = false;
    lightSet = NULL;
    isSelect = false;
    lightDis = 4;
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
        int w = 2*lightDis+1;
        for(it = lightSet->begin(); it != lightSet->end(); it++){
            painter->drawRect((it->first-lightDis)*BLOCK_SIZE, (it->second-lightDis)*BLOCK_SIZE, BLOCK_SIZE*w, BLOCK_SIZE*w);
        }
    }
    if(isSelect && (*selectPoint1IsValid)){
        QColor color(255, 0, 0);
        color.setAlphaF(0.25);
        painter->setBrush(QBrush(color));
        painter->drawRect(selectPoint1->first*BLOCK_SIZE+BLOCK_SIZE/4, selectPoint1->second*BLOCK_SIZE+BLOCK_SIZE/4, BLOCK_SIZE/2, BLOCK_SIZE/2);
    }
    if(isSelect && (*selectPoint2IsValid)){
        QColor color(255, 0, 0);
        color.setAlphaF(0.25);
        painter->setBrush(QBrush(color));
        painter->drawRect(selectPoint2->first*BLOCK_SIZE+BLOCK_SIZE/4, selectPoint2->second*BLOCK_SIZE+BLOCK_SIZE/4, BLOCK_SIZE/2, BLOCK_SIZE/2);
    }
    if(isSelect && (*selectPoint1IsValid) && (*selectPoint2IsValid)){
        int indexX1 = *selectMinX;
        int indexX2 = *selectMaxX;
        int indexY1 = *selectMinY;
        int indexY2 = *selectMaxY;
        int w = indexX2 - indexX1;
        int h = indexY2 - indexY1;
        QColor color(0, 255, 0);
        color.setAlphaF(0.5);
        painter->setBrush(QBrush(color));
        painter->drawRect(indexX1*BLOCK_SIZE, indexY1*BLOCK_SIZE, w*BLOCK_SIZE, h*BLOCK_SIZE);
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
