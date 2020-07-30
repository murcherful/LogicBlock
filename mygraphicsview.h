#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H
#include <QGraphicsView>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QMouseEvent>
#include "common.h"
#include "basicblock.h"
#include "myscene.h"
#include <QDebug>
#include <unordered_map>
#include <QTimer>
#include <algorithm>

typedef std::unordered_map<IndexPair, BasicBlock*, pair_hash> MyHashMap;
typedef std::unordered_set<IndexPair, pair_hash> MyHashSet;

class MyGraphicsView:public QGraphicsView
{
    Q_OBJECT

public:
    MyGraphicsView(QWidget* parent=nullptr);
    void setGraphicsState(int s);
    void setSelectState(int s);
    void startTimer(int breakTime);
    void stopTimer();
    void setBlockType(int type);
    void resetBlock();
    void setMyScene(MyScene* s);
    void deleteSelectArea();
    void pasteSelectArea(int indexX, int indexY);
    void pasteAreaByVector(int indexX, int indexY, std::vector<BlockInfo> blocks);
    std::vector<BlockInfo> getSelectAreaBlockInfo();
    void clearSelectPoint();

    MyScene* myScene;

    static int STATE_EDIT;
    static int STATE_MOVE;
    static int STATE_SELECT;
    static int SELECT_STATE_SET_POINT;
    static int SELECT_STATE_PASTE;
    static int SELECT_STATE_MOVE;

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

    int graphicsState;
    int selectState;
    bool isLeftMouseMoving;

    MyHashMap blockMap;

    QTimer *blockUpdateTimer;

    int blockType;

    MyHashSet* lightSet;
    bool selectPoint1IsValid;
    IndexPair selectPoint1;
    bool selectPoint2IsValid;
    IndexPair selectPoint2;
    int selectMinX;
    int selectMinY;
    int selectMaxX;
    int selectMaxY;

    int selectMoveStartPointX;
    int selectMoveStartPointY;
    IndexPair selectMoveOldPoint1;
    IndexPair selectMoveOldPoint2;
    std::vector<BlockInfo> selectMoveInfo;
    bool isSelectMove;


    void addBlock(int x, int y, int type);
    BasicBlock* getBlock(int x, int y, int type);
    void combineBlock(BasicBlock* bb);
    void deleteBlock(IndexPair index);
    void interactiveBlock(IndexPair index);

    int XYToGrid(int x);
    int XYToIndex(int x);
    int indexToXYCenter(int index);

    void setSelectMaxMinXY();


};

#endif // MYGRAPHICSVIEW_H
