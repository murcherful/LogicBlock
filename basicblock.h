#ifndef BASICBLOCK_H
#define BASICBLOCK_H
#include <QGraphicsItem>
#include <QPainter>
#include <QObject>
#include "common.h"
#include <unordered_set>
#include <QDebug>

typedef std::unordered_set<IndexPair, pair_hash> MyHashSet;

class BasicBlock:public QGraphicsItem
{

public:
    BasicBlock();
    BasicBlock(int tx, int ty);
    QRectF boundingRect() const;
    virtual void updateState();
    virtual void stateSwap();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    virtual void reset();
    virtual void interactive();

    int x;
    int y;

    int indexX;
    int indexY;

    int blockType;

    BasicBlock* upBlock;
    BasicBlock* downBlock;
    BasicBlock* leftBlock;
    BasicBlock* rightBlock;

    int activeState;
    int oldActiveState;
    int upActive;
    int oldUpActive;
    int downActive;
    int oldDownActive;
    int leftActive;
    int oldLeftActive;
    int rightActive;
    int oldRightActive;

    static int POWER;
    static int STRAIGHT_LINE_H;
    static int STRAIGHT_LINE_V;
    static int LEFT_UP_LINE;
    static int LEFT_DOWN_LINE;
    static int RIGHT_UP_LINE;
    static int RIGHT_DOWN_LINE;
    static int LIGHT;
    static int CROSS_CONNECT;
    static int CROSS_NOT_CONNECT;
    static int SWITCH_BLOCK;
    static int AND_GATE_BLOCK;
    static int OR_GATE_BLOCK;
    static int NOT_GATE_BLOCK;
    static int XOR_GATE_BLOCK;
    static int WALL_BLOCK;
    static int LATCH_BLOCK;
    static int ONE_WAY_LINE_UP;
    static int ONE_WAY_LINE_DOWN;
    static int ONE_WAY_LINE_LEFT;
    static int ONE_WAY_LINE_RIGHT;

protected:
    void activeLeft();
    void activeRight();
    void activeUp();
    void activeDown();

private:
    void initArgs();
};

class StraightLineBlock:public BasicBlock
{
public:
    StraightLineBlock();
    StraightLineBlock(int tx, int ty, int ttype);

    void updateState() override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

};

class PowerBlock:public BasicBlock
{
public:
    PowerBlock();
    PowerBlock(int tx, int ty);

    void updateState() override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
};

class CornerBlock:public BasicBlock
{
public:
    CornerBlock();
    CornerBlock(int tx, int ty, int ttype);

    void updateState() override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

private:
    QPointF blockPoints[6];

    void setPoints();

};

class LightBlock:public BasicBlock
{
public:
    LightBlock();
    LightBlock(int tx, int ty, MyHashSet* set);
    ~LightBlock();

    void updateState() override;
    void stateSwap() override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    void reset() override;

private:
    bool isOn;
    bool oldIsOn;

    MyHashSet *lightSet;

    void lightOn();
    void lightOff();

};

class CrossConnectBlock:public BasicBlock
{
public:
    CrossConnectBlock();
    CrossConnectBlock(int tx, int ty);

    void updateState() override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

private:
    QPointF blockPoints[12];

    void setPoints();
};

class CrossNotConnectBlock:public BasicBlock
{
public:
    CrossNotConnectBlock();
    CrossNotConnectBlock(int tx, int ty);

    void updateState() override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    void stateSwap() override;
    void reset() override;

private:
    int activeHState;
    int activeVState;
    int oldActiveHState;
    int oldActiveVState;

    void initArgs();
};

class SwitchBlock:public BasicBlock
{
public:
    SwitchBlock();
    SwitchBlock(int tx, int ty);

    void updateState() override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    void interactive() override;
    void reset() override;
private:
    int isOn;
    void initArgs();
};

class AndGateBlock:public BasicBlock
{
public:
    AndGateBlock();
    AndGateBlock(int tx, int ty);

    void updateState() override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
};

class OrGateBlock:public BasicBlock
{
public:
    OrGateBlock();
    OrGateBlock(int tx, int ty);

    void updateState() override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
};

class NotGateBlock:public BasicBlock
{
public:
    NotGateBlock();
    NotGateBlock(int tx, int ty);

    void updateState() override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
};

class XorGateBlock:public BasicBlock
{
public:
    XorGateBlock();
    XorGateBlock(int tx, int ty);

   void updateState() override;
   void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
};

class WallBlock:public BasicBlock
{
public:
    WallBlock();
    WallBlock(int tx, int ty);

    void updateState() override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
};

class LatchBlock:public BasicBlock
{
public:
    LatchBlock();
    LatchBlock(int tx, int ty);

    void updateState() override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    void reset() override;
    void stateSwap() override;

private:
    int storeState;
    int oldStoreState;
    void initArgs();
};

class OneWayLine:public BasicBlock
{
public:
    OneWayLine();
    OneWayLine(int tx, int ty, int type);

    void updateState() override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

private:
    QPointF blockPoints[3];
    void setPoints();
};

#endif // BASICBLOCK_H
