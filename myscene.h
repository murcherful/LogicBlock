#ifndef MYSCENE_H
#define MYSCENE_H
#include <QGraphicsView>
#include "common.h"
typedef std::unordered_set<IndexPair, pair_hash> MyHashSet;

class MyScene:public QGraphicsScene
{
public:
    MyScene(QWidget *parent=nullptr);

    void drawForeground(QPainter *painter, const QRectF &rect) override;
    void drawBackground(QPainter *painter, const QRectF &rect) override;
    void setIsDrawGrid(bool s);
    void setIsNight(bool s);
    void setIsRenderLight(bool s);
    MyHashSet *lightSet;
    bool* selectPoint1IsValid;
    bool* selectPoint2IsValid;
    IndexPair* selectPoint1;
    IndexPair* selectPoint2;
    int* selectMinX;
    int* selectMaxX;
    int* selectMinY;
    int* selectMaxY;
    bool isSelect;

private:
    bool isDrawGrid;
    bool isNight;
    bool isRenderLight;


    void drawGrid(QPainter *painter, const QRectF &rect);

};

#endif // MYSCENE_H
