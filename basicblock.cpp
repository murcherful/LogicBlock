#include "basicblock.h"

int BasicBlock::POWER = 0;
int BasicBlock::STRAIGHT_LINE_H = 1;
int BasicBlock::STRAIGHT_LINE_V = 2;
int BasicBlock::LEFT_UP_LINE = 3;
int BasicBlock::LEFT_DOWN_LINE = 4;
int BasicBlock::RIGHT_UP_LINE = 5;
int BasicBlock::RIGHT_DOWN_LINE = 6;
int BasicBlock::LIGHT = 7;
int BasicBlock::CROSS_CONNECT = 8;
int BasicBlock::CROSS_NOT_CONNECT = 9;
int BasicBlock::SWITCH_BLOCK = 10;
int BasicBlock::AND_GATE_BLOCK = 11;
int BasicBlock::OR_GATE_BLOCK = 12;
int BasicBlock::NOT_GATE_BLOCK = 13;
int BasicBlock::XOR_GATE_BLOCK = 14;

void BasicBlock::initArgs(){
    x = 0;
    y = 0;
    indexX = 0;
    indexY = 0;

    upBlock = NULL;
    downBlock = NULL;
    leftBlock = NULL;
    rightBlock = NULL;

    reset();
}

BasicBlock::BasicBlock(){
    initArgs();
}

BasicBlock::BasicBlock(int tx, int ty)
{
    initArgs();
    x = tx;
    y = ty;
    indexX = tx / BLOCK_SIZE;
    indexY = ty / BLOCK_SIZE;
}

void BasicBlock::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    if(oldActiveState){
        painter->setBrush(QBrush(QColor(255, 0, 0)));
    }
    else{
        painter->setBrush(QBrush(QColor(0, 255, 0)));
    }
    painter->drawRect(x, y, BLOCK_SIZE, BLOCK_SIZE);
}

QRectF BasicBlock::boundingRect() const{
    return QRectF(x, y, BLOCK_SIZE, BLOCK_SIZE);
}

void BasicBlock::updateState(){
    if(oldLeftActive){
        activeState = 1;
        activeRight();
    }
    else if(oldRightActive){
        activeState = 1;
        activeLeft();
    }
}

void BasicBlock::stateSwap(){
    oldActiveState = activeState;
    activeState = 0;
    oldLeftActive = leftActive;
    leftActive = 0;
    oldRightActive = rightActive;
    rightActive = 0;
    oldUpActive = upActive;
    upActive = 0;
    oldDownActive = downActive;
    downActive = 0;
}

void BasicBlock::reset(){
    activeState = 0;
    oldActiveState = 0;
    upActive = 0;
    oldUpActive = 0;
    downActive = 0;
    oldDownActive = 0;
    leftActive = 0;
    oldLeftActive = 0;
    rightActive = 0;
    oldRightActive = 0;
}

void BasicBlock::activeLeft(){
    if(leftBlock){
        leftBlock->rightActive = 1;
    }
}

void BasicBlock::activeRight(){
    if(rightBlock){
        rightBlock->leftActive = 1;
    }
}

void BasicBlock::activeUp(){
    if(upBlock){
        upBlock->downActive = 1;
    }
}

void BasicBlock::activeDown(){
    if(downBlock){
        downBlock->upActive = 1;
    }
}
void BasicBlock::interactive(){
    return;
}

StraightLineBlock::StraightLineBlock():BasicBlock(){
    blockType = STRAIGHT_LINE_H;
}

StraightLineBlock::StraightLineBlock(int tx, int ty, int ttype):BasicBlock(tx, ty){
    blockType = ttype;
}

void StraightLineBlock::updateState(){
    if(blockType == STRAIGHT_LINE_H){
        if(oldLeftActive){
            activeState = 1;
            activeRight();
        }
        else if(oldRightActive){
            activeState = 1;
            activeLeft();
        }
    }
    else if(blockType == STRAIGHT_LINE_V){
        if(oldUpActive){
            activeState = 1;
            activeDown();
        }
        else if(oldDownActive){
            activeState = 1;
            activeUp();
        }
    }
}

void StraightLineBlock::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    painter->setBrush(QBrush(QColor(211, 211, 211)));
    painter->drawRect(x, y, BLOCK_SIZE, BLOCK_SIZE);
    if(oldActiveState){
        painter->setBrush(QBrush(QColor(255, 0, 0)));
    }
    else{
        painter->setBrush(QBrush(QColor(139, 0, 0)));
    }
    if(blockType == STRAIGHT_LINE_H){
        painter->drawRect(x, y+BLOCK_SIZE/4, BLOCK_SIZE, BLOCK_SIZE/2);
    }
    else if(blockType == STRAIGHT_LINE_V){
        painter->drawRect(x+BLOCK_SIZE/4, y, BLOCK_SIZE/2, BLOCK_SIZE);
    }
}

PowerBlock::PowerBlock():BasicBlock(){
    blockType = POWER;
}

PowerBlock::PowerBlock(int tx, int ty):BasicBlock(tx, ty){
    blockType = POWER;
}

void PowerBlock::updateState(){
    activeUp();
    activeDown();
    activeLeft();
    activeRight();
}

void PowerBlock::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    painter->setBrush(QBrush(QColor(211, 211, 211)));
    painter->drawRect(x, y, BLOCK_SIZE, BLOCK_SIZE);
    painter->setBrush(QBrush(QColor(255, 0, 0)));
    painter->drawRect(x, y+BLOCK_SIZE/4, BLOCK_SIZE, BLOCK_SIZE/2);
    painter->drawRect(x+BLOCK_SIZE/4, y, BLOCK_SIZE/2, BLOCK_SIZE);
    painter->setBrush(QBrush(QColor(255, 140, 0)));
    painter->drawRect(x+BLOCK_SIZE/4, y+BLOCK_SIZE/4, BLOCK_SIZE/2, BLOCK_SIZE/2);
    painter->setBrush(QBrush(QColor(255, 255, 0)));
    painter->drawRect(x+3*BLOCK_SIZE/8, y+3*BLOCK_SIZE/8, BLOCK_SIZE/4, BLOCK_SIZE/4);
}

CornerBlock::CornerBlock():BasicBlock(){
    blockType = LEFT_UP_LINE;
    setPoints();
}

CornerBlock::CornerBlock(int tx, int ty, int ttype):BasicBlock(tx, ty){
    blockType = ttype;
    setPoints();
}

void CornerBlock::updateState(){
    if(blockType == LEFT_UP_LINE){
        if(oldLeftActive){
            activeState = 1;
            activeUp();
        }
        else if(oldUpActive){
            activeState = 1;
            activeLeft();
        }
    }
    else if(blockType == LEFT_DOWN_LINE){
        if(oldDownActive){
            activeState = 1;
            activeLeft();
        }
        else if(oldLeftActive){
            activeState = 1;
            activeDown();
        }
    }
    else if(blockType == RIGHT_UP_LINE){
        if(oldRightActive){
            activeState = 1;
            activeUp();
        }
        else if(oldUpActive){
            activeState = 1;
            activeRight();
        }
    }
    else if(blockType == RIGHT_DOWN_LINE){
        if(oldDownActive){
            activeState = 1;
            activeRight();
        }
        else if(oldRightActive){
            activeState = 1;
            activeDown();
        }
    }
}

void CornerBlock::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    painter->setBrush(QBrush(QColor(211, 211, 211)));
    painter->drawRect(x, y, BLOCK_SIZE, BLOCK_SIZE);
    if(oldActiveState){
        painter->setBrush(QBrush(QColor(255, 0, 0)));
    }
    else{
        painter->setBrush(QBrush(QColor(139, 0, 0)));
    }
    painter->drawPolygon(blockPoints, 6);
}

void CornerBlock::setPoints(){
    if(blockType == LEFT_UP_LINE){
        blockPoints[0] = QPointF(x, y+BLOCK_SIZE/4);
        blockPoints[1] = QPointF(x+BLOCK_SIZE/4, y+BLOCK_SIZE/4);
        blockPoints[2] = QPointF(x+BLOCK_SIZE/4, y);
        blockPoints[3] = QPointF(x+3*BLOCK_SIZE/4, y);
        blockPoints[4] = QPointF(x+3*BLOCK_SIZE/4, y+3*BLOCK_SIZE/4);
        blockPoints[5] = QPointF(x, y+3*BLOCK_SIZE/4);
    }
    else if(blockType == LEFT_DOWN_LINE){
        blockPoints[0] = QPointF(x, y+BLOCK_SIZE/4);
        blockPoints[1] = QPointF(x+3*BLOCK_SIZE/4, y+BLOCK_SIZE/4);
        blockPoints[2] = QPointF(x+3*BLOCK_SIZE/4, y+BLOCK_SIZE);
        blockPoints[3] = QPointF(x+BLOCK_SIZE/4, y+BLOCK_SIZE);
        blockPoints[4] = QPointF(x+BLOCK_SIZE/4, y+3*BLOCK_SIZE/4);
        blockPoints[5] = QPointF(x, y+3*BLOCK_SIZE/4);
    }
    else if(blockType == RIGHT_UP_LINE){
        blockPoints[0] = QPointF(x+BLOCK_SIZE/4, y);
        blockPoints[1] = QPointF(x+3*BLOCK_SIZE/4, y);
        blockPoints[2] = QPointF(x+3*BLOCK_SIZE/4, y+BLOCK_SIZE/4);
        blockPoints[3] = QPointF(x+BLOCK_SIZE, y+BLOCK_SIZE/4);
        blockPoints[4] = QPointF(x+BLOCK_SIZE, y+3*BLOCK_SIZE/4);
        blockPoints[5] = QPointF(x+BLOCK_SIZE/4, y+3*BLOCK_SIZE/4);
    }
    else if(blockType == RIGHT_DOWN_LINE){
        blockPoints[0] = QPointF(x+BLOCK_SIZE/4, y+BLOCK_SIZE/4);
        blockPoints[1] = QPointF(x+BLOCK_SIZE, y+BLOCK_SIZE/4);
        blockPoints[2] = QPointF(x+BLOCK_SIZE, y+3*BLOCK_SIZE/4);
        blockPoints[3] = QPointF(x+3*BLOCK_SIZE/4, y+3*BLOCK_SIZE/4);
        blockPoints[4] = QPointF(x+3*BLOCK_SIZE/4, y+BLOCK_SIZE);
        blockPoints[5] = QPointF(x+BLOCK_SIZE/4, y+BLOCK_SIZE);
    }
}


LightBlock::LightBlock():BasicBlock(){
    isOn = false;
    oldIsOn = false;
    lightSet = NULL;
    blockType = LIGHT;
}

LightBlock::LightBlock(int tx, int ty, MyHashSet* set):BasicBlock(tx, ty){
    isOn = false;
    oldIsOn = false;
    lightSet = set;
    blockType = LIGHT;
}

void LightBlock::updateState(){
    if(oldUpActive){
        activeState = 1;
        isOn = 1;
    }
    else if(oldDownActive){
        activeState = 1;
        isOn = 1;
    }
    else if(oldLeftActive){
        activeState = 1;
        isOn = 1;
    }
    else if(oldRightActive){
        activeState = 1;
        isOn = 1;
    }
}

void LightBlock::stateSwap(){
    BasicBlock::stateSwap();
    if(isOn){
        if(!oldIsOn){
            oldIsOn = 1;
            lightOn();
        }
        isOn = 0;
    }
    else{
        if(oldIsOn){
            oldIsOn = 0;
            lightOff();
        }
    }
}

void LightBlock::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    if(oldActiveState){
        painter->setBrush(QBrush(QColor(255, 255, 0)));
    }
    else{
        painter->setBrush(QBrush(QColor(210, 180, 140)));
    }
    painter->drawRect(x, y, BLOCK_SIZE, BLOCK_SIZE);
}

void LightBlock::lightOn(){
    IndexPair index(indexX, indexY);
    MyHashSet::iterator it = lightSet->find(index);
    if(it == lightSet->end()){
        lightSet->insert(index);
    }
}

void LightBlock::lightOff(){
    IndexPair index(indexX, indexY);
    MyHashSet::iterator it = lightSet->find(index);
    if(it != lightSet->end()){
        lightSet->erase(it);
    }
}

LightBlock::~LightBlock(){
    lightOff();
}

CrossConnectBlock::CrossConnectBlock():BasicBlock(){
    setPoints();
    blockType = CROSS_CONNECT;
}

CrossConnectBlock::CrossConnectBlock(int tx, int ty):BasicBlock(tx, ty){
    setPoints();
    blockType = CROSS_CONNECT;
}

void CrossConnectBlock::updateState(){
    if(oldUpActive){
        activeState = 1;
        activeLeft();
        activeRight();
        activeDown();
    }
    else if(oldDownActive){
        activeState = 1;
        activeLeft();
        activeRight();
        activeUp();
    }
    else if(oldLeftActive){
        activeState = 1;
        activeUp();
        activeDown();
        activeRight();
    }
    else if(oldRightActive){
        activeState = 1;
        activeUp();
        activeDown();
        activeLeft();
    }
}

void CrossConnectBlock::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    painter->setBrush(QBrush(QColor(211, 211, 211)));
    painter->drawRect(x, y, BLOCK_SIZE, BLOCK_SIZE);
    if(oldActiveState){
        painter->setBrush(QBrush(QColor(255, 0, 0)));
    }
    else{
        painter->setBrush(QBrush(QColor(139, 0, 0)));
    }
    painter->drawPolygon(blockPoints, 12);
}

void CrossConnectBlock::setPoints(){
    blockPoints[0] = QPointF(x, y+BLOCK_SIZE/4);
    blockPoints[1] = QPointF(x+BLOCK_SIZE/4, y+BLOCK_SIZE/4);
    blockPoints[2] = QPointF(x+BLOCK_SIZE/4, y);
    blockPoints[3] = QPointF(x+3*BLOCK_SIZE/4, y);
    blockPoints[4] = QPointF(x+3*BLOCK_SIZE/4, y+BLOCK_SIZE/4);
    blockPoints[5] = QPointF(x+BLOCK_SIZE, y+BLOCK_SIZE/4);
    blockPoints[6] = QPointF(x+BLOCK_SIZE, y+3*BLOCK_SIZE/4);
    blockPoints[7] = QPointF(x+3*BLOCK_SIZE/4, y+3*BLOCK_SIZE/4);
    blockPoints[8] = QPointF(x+3*BLOCK_SIZE/4, y+BLOCK_SIZE);
    blockPoints[9] = QPointF(x+BLOCK_SIZE/4, y+BLOCK_SIZE);
    blockPoints[10] = QPointF(x+BLOCK_SIZE/4, y+3*BLOCK_SIZE/4);
    blockPoints[11] = QPointF(x, y+3*BLOCK_SIZE/4);
}

CrossNotConnectBlock::CrossNotConnectBlock():BasicBlock(){
    initArgs();
    blockType = CROSS_NOT_CONNECT;
}

CrossNotConnectBlock::CrossNotConnectBlock(int tx, int ty):BasicBlock(tx, ty){
    initArgs();
    blockType = CROSS_NOT_CONNECT;
}

void CrossNotConnectBlock::updateState(){
    if(oldUpActive){
        activeVState = 1;
        activeDown();
    }
    else if(oldDownActive){
        activeVState = 1;
        activeUp();
    }
    if(oldLeftActive){
        activeHState = 1;
        activeRight();
    }
    else if(oldRightActive){
        activeHState = 1;
        activeLeft();
    }
}

void CrossNotConnectBlock::stateSwap(){
    BasicBlock::stateSwap();
    oldActiveHState = activeHState;
    oldActiveVState = activeVState;
    activeHState = 0;
    activeVState = 0;
}

void CrossNotConnectBlock::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    painter->setBrush(QBrush(QColor(211, 211, 211)));
    painter->drawRect(x, y, BLOCK_SIZE, BLOCK_SIZE);
    if(oldActiveVState){
        painter->setBrush(QBrush(QColor(255, 0, 0)));
    }
    else{
        painter->setBrush(QBrush(QColor(139, 0, 0)));
    }
    painter->drawRect(x+BLOCK_SIZE/4, y, BLOCK_SIZE/2, BLOCK_SIZE);
    if(oldActiveHState){
        painter->setBrush(QBrush(QColor(255, 0, 0)));
    }
    else{
        painter->setBrush(QBrush(QColor(139, 0, 0)));
    }
    painter->drawRect(x, y+BLOCK_SIZE/4, BLOCK_SIZE, BLOCK_SIZE/2);
}

void CrossNotConnectBlock::reset(){
    BasicBlock::reset();
    initArgs();
}

void CrossNotConnectBlock::initArgs(){
    activeHState = 0;
    activeVState = 0;
    oldActiveHState = 0;
    oldActiveVState = 0;
}

SwitchBlock::SwitchBlock():BasicBlock(){
    initArgs();
    blockType = SWITCH_BLOCK;
}

SwitchBlock::SwitchBlock(int tx, int ty):BasicBlock(tx, ty){
    initArgs();
    blockType = SWITCH_BLOCK;
}

void SwitchBlock::initArgs(){
    isOn = 0;
}

void SwitchBlock::reset(){
    BasicBlock::reset();
    isOn = 0;
}

void SwitchBlock::updateState(){
    if(isOn){
        activeUp();
        activeDown();
        activeLeft();
        activeRight();
    }
}

void SwitchBlock::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    painter->setBrush(QBrush(QColor(211, 211, 211)));
    painter->drawRect(x, y, BLOCK_SIZE, BLOCK_SIZE);
    if(isOn){
        painter->setBrush(QBrush(QColor(255, 0, 0)));
    }
    else{
        painter->setBrush(QBrush(QColor(139, 0, 0)));
    }
    painter->drawEllipse(x, y, BLOCK_SIZE, BLOCK_SIZE);
}

void SwitchBlock::interactive(){
    if(isOn){
        isOn = 0;
    }
    else{
        isOn = 1;
    }
}

AndGateBlock::AndGateBlock():BasicBlock(){
    blockType = AND_GATE_BLOCK;
}

AndGateBlock::AndGateBlock(int tx, int ty):BasicBlock(tx, ty){
    blockType = AND_GATE_BLOCK;
}

void AndGateBlock::updateState(){
    if(oldUpActive && oldDownActive){
        activeState = 1;
        activeRight();
    }
}

void AndGateBlock::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    painter->setBrush(QBrush(QColor(211, 211, 211)));
    painter->drawRect(x, y, BLOCK_SIZE, BLOCK_SIZE);
    if(oldActiveState){
        painter->setBrush(QBrush(QColor(255, 0, 0)));
    }
    else{
        painter->setBrush(QBrush(QColor(139, 0, 0)));
    }
    painter->drawRect(x+BLOCK_SIZE/4, y, BLOCK_SIZE/2, BLOCK_SIZE);
    painter->drawRect(x+BLOCK_SIZE/4, y+BLOCK_SIZE/4, 3*BLOCK_SIZE/4, BLOCK_SIZE/2);
    if(oldActiveState){
        painter->setPen(QPen(QColor(0, 255, 255)));
    }
    else{
        painter->setPen(QPen(QColor(255-139, 255, 255)));
    }
    QFont font;
    font.setFamily("Microsoft YaHei");
    font.setPointSize(BLOCK_SIZE/6);
    font.setBold(true);
    painter->setFont(font);
    painter->drawText(x+BLOCK_SIZE/4, y+BLOCK_SIZE/4, 3*BLOCK_SIZE/4, BLOCK_SIZE/2, Qt::AlignHCenter|Qt::AlignVCenter, "AND");
}

OrGateBlock::OrGateBlock():BasicBlock(){
    blockType = OR_GATE_BLOCK;
}

OrGateBlock::OrGateBlock(int tx, int ty):BasicBlock(tx, ty){
    blockType = OR_GATE_BLOCK;
}

void OrGateBlock::updateState(){
    if(oldUpActive || oldDownActive){
        activeState = 1;
        activeRight();
    }
}

void OrGateBlock::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    painter->setBrush(QBrush(QColor(211, 211, 211)));
    painter->drawRect(x, y, BLOCK_SIZE, BLOCK_SIZE);
    if(oldActiveState){
        painter->setBrush(QBrush(QColor(255, 0, 0)));
    }
    else{
        painter->setBrush(QBrush(QColor(139, 0, 0)));
    }
    painter->drawRect(x+BLOCK_SIZE/4, y, BLOCK_SIZE/2, BLOCK_SIZE);
    painter->drawRect(x+BLOCK_SIZE/4, y+BLOCK_SIZE/4, 3*BLOCK_SIZE/4, BLOCK_SIZE/2);
    if(oldActiveState){
        painter->setPen(QPen(QColor(0, 255, 255)));
    }
    else{
        painter->setPen(QPen(QColor(255-139, 255, 255)));
    }
    QFont font;
    font.setFamily("Microsoft YaHei");
    font.setPointSize(BLOCK_SIZE/6);
    font.setBold(true);
    painter->setFont(font);
    painter->drawText(x+BLOCK_SIZE/4, y+BLOCK_SIZE/4, 3*BLOCK_SIZE/4, BLOCK_SIZE/2, Qt::AlignHCenter|Qt::AlignVCenter, "OR");
}

NotGateBlock::NotGateBlock():BasicBlock(){
    blockType = NOT_GATE_BLOCK;
}

NotGateBlock::NotGateBlock(int tx, int ty):BasicBlock(tx, ty){
    blockType = NOT_GATE_BLOCK;
}

void NotGateBlock::updateState(){
    if(!oldLeftActive){
        activeState = 1;
        activeRight();
    }
}

void NotGateBlock::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    painter->setBrush(QBrush(QColor(211, 211, 211)));
    painter->drawRect(x, y, BLOCK_SIZE, BLOCK_SIZE);
    if(oldActiveState){
        painter->setBrush(QBrush(QColor(255, 0, 0)));
    }
    else{
        painter->setBrush(QBrush(QColor(139, 0, 0)));
    }
    painter->drawRect(x, y+BLOCK_SIZE/4, BLOCK_SIZE, BLOCK_SIZE/2);
    if(oldActiveState){
        painter->setPen(QPen(QColor(0, 255, 255)));
    }
    else{
        painter->setPen(QPen(QColor(255-139, 255, 255)));
    }
    QFont font;
    font.setFamily("Microsoft YaHei");
    font.setPointSize(BLOCK_SIZE/6);
    font.setBold(true);
    painter->setFont(font);
    painter->drawText(x, y+BLOCK_SIZE/4, BLOCK_SIZE, BLOCK_SIZE/2, Qt::AlignHCenter|Qt::AlignVCenter, "NOT");
}

XorGateBlock::XorGateBlock():BasicBlock(){
    blockType = XOR_GATE_BLOCK;
}

XorGateBlock::XorGateBlock(int tx, int ty):BasicBlock(tx, ty){
    blockType = XOR_GATE_BLOCK;
}

void XorGateBlock::updateState(){
    if(oldUpActive != oldDownActive){
        activeState = 1;
        activeRight();
    }
}

void XorGateBlock::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    painter->setBrush(QBrush(QColor(211, 211, 211)));
    painter->drawRect(x, y, BLOCK_SIZE, BLOCK_SIZE);
    if(oldActiveState){
        painter->setBrush(QBrush(QColor(255, 0, 0)));
    }
    else{
        painter->setBrush(QBrush(QColor(139, 0, 0)));
    }
    painter->drawRect(x+BLOCK_SIZE/4, y, BLOCK_SIZE/2, BLOCK_SIZE);
    painter->drawRect(x+BLOCK_SIZE/4, y+BLOCK_SIZE/4, 3*BLOCK_SIZE/4, BLOCK_SIZE/2);
    if(oldActiveState){
        painter->setPen(QPen(QColor(0, 255, 255)));
    }
    else{
        painter->setPen(QPen(QColor(255-139, 255, 255)));
    }
    QFont font;
    font.setFamily("Microsoft YaHei");
    font.setPointSize(BLOCK_SIZE/6);
    font.setBold(true);
    painter->setFont(font);
    painter->drawText(x+BLOCK_SIZE/4, y+BLOCK_SIZE/4, 3*BLOCK_SIZE/4, BLOCK_SIZE/2, Qt::AlignHCenter|Qt::AlignVCenter, "XOR");
}
