#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H
#include <QGraphicsView>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QMouseEvent>
#include "common.h"
#include "basicblock.h"
#include <QDebug>
#include <unordered_map>
#include <QTimer>

typedef std::unordered_map<IndexPair, BasicBlock*, pair_hash> MyHashMap;
typedef std::unordered_set<IndexPair, pair_hash> MyHashSet;

class MyGraphicsView:public QGraphicsView
{
    Q_OBJECT

public:
    MyGraphicsView(QWidget* parent=nullptr);
    void setIsEdit(bool s);
    void startTimer(int breakTime);
    void stopTimer();
    void setBlockType(int type);
    void resetBlock();

    MyHashSet* lightSet;

protected:
    void keyPressEvent(QKeyEvent* event);
    void wheelEvent(QWheelEvent* event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent* event);

signals:
    void graphicsMove();
    void graphicsEdit();

private slots:
    void blockUpdate();

private:
    double zoomScale;

    int centerX;
    int centerY;

    QPointF startPoint;

    bool isEdit;
    bool isLeftMouseMoving;

    MyHashMap blockMap;

    QTimer *blockUpdateTimer;

    int blockType;

    void addBlock(int x, int y);
    BasicBlock* getBlock(int x, int y);
    void combineBlock(BasicBlock* bb);
    void deleteBlock(IndexPair index);
    void interactiveBlock(IndexPair index);

    int transToGrid(int x);
    int transToIndex(int x);

};

#endif // MYGRAPHICSVIEW_H
