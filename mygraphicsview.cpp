#include "mygraphicsview.h"

int MyGraphicsView::STATE_EDIT = 0;
int MyGraphicsView::STATE_MOVE = 1;
int MyGraphicsView::STATE_SELECT = 2;

int MyGraphicsView::SELECT_STATE_SET_POINT = 0;
int MyGraphicsView::SELECT_STATE_PASTE = 1;
int MyGraphicsView::SELECT_STATE_MOVE = 2;

MyGraphicsView::MyGraphicsView(QWidget* parent):QGraphicsView(parent){
    zoomScale = 1;
    centerX = 0;
    centerY = 0;
    graphicsState = STATE_MOVE;
    selectState = SELECT_STATE_SET_POINT;
    isLeftMouseMoving = false;
    this->setCursor(Qt::SizeAllCursor);
    blockUpdateTimer = new QTimer(this);
    connect(blockUpdateTimer, SIGNAL(timeout()), this, SLOT(blockUpdate()));
    blockType = BasicBlock::POWER;
    lightSet = new MyHashSet;
    selectPoint1IsValid = false;
    selectPoint2IsValid = false;
    isSelectMove = false;
}

void MyGraphicsView::keyPressEvent(QKeyEvent *event){
    int moveStep = KEY_MOVE_SPEED/zoomScale;
    if(event->key() == Qt::Key_A){
        centerX -= moveStep;
        scene()->setSceneRect(centerX, centerY, SCENE_WIDTH, SCENE_HEIGHT);
        scene()->update();
    }
    else if(event->key() == Qt::Key_D){
        centerX += moveStep;
        scene()->setSceneRect(centerX, centerY, SCENE_WIDTH, SCENE_HEIGHT);
        scene()->update();
    }
    else if(event->key() == Qt::Key_W){
        centerY -= moveStep;
        scene()->setSceneRect(centerX, centerY, SCENE_WIDTH, SCENE_HEIGHT);
        scene()->update();
    }
    else if(event->key() == Qt::Key_S){
        centerY += moveStep;
        scene()->setSceneRect(centerX, centerY, SCENE_WIDTH, SCENE_HEIGHT);
        scene()->update();
    }
    if(event->key() == Qt::Key_E){
        graphicsEdit();
    }
    else if(event->key() == Qt::Key_M){
        graphicsMove();
    }
}

void MyGraphicsView::wheelEvent(QWheelEvent *event){
    int numValue = event->delta();
    double numDegrees = numValue / 8.0;
    double numSteps = numDegrees / 15.0;
    double factor = 1 + numSteps * WHELL_SCALE_SPEED;
    if(zoomScale * factor > 0.05 && zoomScale * factor < 4){
        zoomScale *= factor;
        scale(factor, factor);
    }
    QWidget::wheelEvent(event);
}

void MyGraphicsView::mousePressEvent(QMouseEvent *event){
    QPointF scenePoint = this->mapToScene(event->pos());
    int indexX = XYToIndex(scenePoint.x());
    int indexY = XYToIndex(scenePoint.y());
    IndexPair index(indexX, indexY);
    if(event->button() == Qt::LeftButton){
        if(graphicsState == STATE_EDIT){
            QPointF scenePoint = this->mapToScene(event->pos());
            addBlock(scenePoint.x(), scenePoint.y(), blockType);
        }
        else if(graphicsState == STATE_MOVE){
            startPoint = event->pos();
            isLeftMouseMoving = true;
        }
        else if(graphicsState == STATE_SELECT){
            if(selectState == SELECT_STATE_SET_POINT){
                selectPoint1IsValid = true;
                selectPoint1.first = indexX;
                selectPoint1.second = indexY;
                setSelectMaxMinXY();
            }
            else if(selectState == SELECT_STATE_PASTE){
                pasteSelectArea(indexX, indexY);
            }
            else if(selectState == SELECT_STATE_MOVE){
                selectMoveStartPointX = indexX;
                selectMoveStartPointY = indexY;
                selectMoveOldPoint1 = selectPoint1;
                selectMoveOldPoint2 = selectPoint2;
                selectMoveInfo = getSelectAreaBlockInfo();
                deleteSelectArea();
                isSelectMove = true;
            }
            myScene->update();
        }
    }
    else if(event->button() == Qt::RightButton){
        if(graphicsState == STATE_EDIT){
            deleteBlock(index);
        }
        else if(graphicsState == STATE_MOVE){
            interactiveBlock(index);
        }
        else if(graphicsState == STATE_SELECT){
            if(selectState == SELECT_STATE_SET_POINT){
                selectPoint2IsValid = true;
                selectPoint2.first = indexX;
                selectPoint2.second = indexY;
                setSelectMaxMinXY();
            }
            myScene->update();
        }
    }
}

void MyGraphicsView::mouseMoveEvent(QMouseEvent *event){
    if(isLeftMouseMoving){
        QPointF disPoint = event->pos() - startPoint;
        disPoint /= zoomScale;
        scene()->setSceneRect(centerX-disPoint.x(), centerY-disPoint.y(), SCENE_WIDTH, SCENE_HEIGHT);
        scene()->update();
    }
    if(isSelectMove){
        QPointF scenePoint = this->mapToScene(event->pos());
        int indexX = XYToIndex(scenePoint.x());
        int indexY = XYToIndex(scenePoint.y());
        IndexPair index(indexX, indexY);
        selectPoint1.first = indexX - selectMoveStartPointX + selectMoveOldPoint1.first;
        selectPoint1.second = indexY - selectMoveStartPointY + selectMoveOldPoint1.second;
        selectPoint2.first = indexX - selectMoveStartPointX + selectMoveOldPoint2.first;
        selectPoint2.second = indexY - selectMoveStartPointY + selectMoveOldPoint2.second;
        setSelectMaxMinXY();
        scene()->update();
    }
}

void MyGraphicsView::mouseReleaseEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        if(graphicsState == STATE_EDIT){

        }
        else if(graphicsState == STATE_MOVE){
            QPointF disPoint = event->pos() - startPoint;
            disPoint /= zoomScale;
            scene()->setSceneRect(centerX-disPoint.x(), centerY-disPoint.y(), SCENE_WIDTH, SCENE_HEIGHT);
            scene()->update();
            centerX = centerX-disPoint.x();
            centerY = centerY-disPoint.y();
            isLeftMouseMoving = false;
        }
        else if(graphicsState == STATE_SELECT){
            if(selectState == SELECT_STATE_MOVE && isSelectMove){
                QPointF scenePoint = this->mapToScene(event->pos());
                int indexX = XYToIndex(scenePoint.x());
                int indexY = XYToIndex(scenePoint.y());
                IndexPair index(indexX, indexY);
                selectPoint1.first = indexX - selectMoveStartPointX + selectMoveOldPoint1.first;
                selectPoint1.second = indexY - selectMoveStartPointY + selectMoveOldPoint1.second;
                selectPoint2.first = indexX - selectMoveStartPointX + selectMoveOldPoint2.first;
                selectPoint2.second = indexY - selectMoveStartPointY + selectMoveOldPoint2.second;
                setSelectMaxMinXY();
                isSelectMove = false;
                pasteAreaByVector(selectMinX, selectMinY, selectMoveInfo);
                myScene->update();
            }
        }
    }
}

void MyGraphicsView::setGraphicsState(int s){
    graphicsState = s;
    if(graphicsState == STATE_MOVE){
        this->setCursor(Qt::SizeAllCursor);
    }
    else{
        this->setCursor(Qt::ArrowCursor);
    }
    if(graphicsState == STATE_SELECT){
        myScene->isSelect = true;
    }
    else{
        myScene->isSelect = false;
    }
    myScene->update();
}

void MyGraphicsView::setSelectState(int s){
    selectState = s;
}

void MyGraphicsView::addBlock(int x, int y, int type){
    x = XYToGrid(x);
    y = XYToGrid(y);
    int indexX = x / BLOCK_SIZE;
    int indexY = y / BLOCK_SIZE;
    IndexPair index(indexX, indexY);
    MyHashMap::iterator it = blockMap.find(index);
    if(it != blockMap.end()){
       return;
    }
    BasicBlock* bb = getBlock(x, y, type);
    blockMap.insert(std::pair<IndexPair, BasicBlock*>(index, bb));
    combineBlock(bb);
    this->scene()->addItem(bb);
    this->scene()->update();
}

BasicBlock* MyGraphicsView::getBlock(int x, int y, int type){
    if(type == BasicBlock::POWER){
        return new PowerBlock(x, y);
    }
    else if(type == BasicBlock::STRAIGHT_LINE_H){
        return new StraightLineBlock(x, y, type);
    }
    else if(type == BasicBlock::STRAIGHT_LINE_V){
        return new StraightLineBlock(x, y, type);
    }
    else if(type == BasicBlock::LEFT_UP_LINE){
        return new CornerBlock(x, y, type);
    }
    else if(type == BasicBlock::LEFT_DOWN_LINE){
        return new CornerBlock(x, y, type);
    }
    else if(type == BasicBlock::RIGHT_UP_LINE){
        return new CornerBlock(x, y, type);
    }
    else if(type == BasicBlock::RIGHT_DOWN_LINE){
        return new CornerBlock(x, y, type);
    }
    else if(type == BasicBlock::LIGHT){
        return new LightBlock(x, y, lightSet);
    }
    else if(type == BasicBlock::CROSS_CONNECT){
        return new CrossConnectBlock(x, y);
    }
    else if(type == BasicBlock::CROSS_NOT_CONNECT){
        return new CrossNotConnectBlock(x, y);
    }
    else if(type == BasicBlock::SWITCH_BLOCK){
        return new SwitchBlock(x, y);
    }
    else if(type == BasicBlock::AND_GATE_BLOCK){
        return new AndGateBlock(x, y);
    }
    else if(type == BasicBlock::OR_GATE_BLOCK){
        return new OrGateBlock(x, y);
    }
    else if(type == BasicBlock::NOT_GATE_BLOCK){
        return new NotGateBlock(x, y);
    }
    else if(type == BasicBlock::XOR_GATE_BLOCK){
        return new XorGateBlock(x, y);
    }
}

void MyGraphicsView::combineBlock(BasicBlock *bb){
    int indexX = bb->indexX;
    int indexY = bb->indexY;
    IndexPair index;
    MyHashMap::iterator it;

    // up
    index = IndexPair(indexX, indexY-1);
    it = blockMap.find(index);
    if(it != blockMap.end()){
        it->second->downBlock = bb;
        bb->upBlock = it->second;
    }

    // down
    index = IndexPair(indexX, indexY+1);
    it = blockMap.find(index);
    if(it != blockMap.end()){
        it->second->upBlock = bb;
        bb->downBlock = it->second;
    }

    // left
    index = IndexPair(indexX-1, indexY);
    it = blockMap.find(index);
    if(it != blockMap.end()){
        it->second->rightBlock = bb;
        bb->leftBlock = it->second;
    }

    // right
    index = IndexPair(indexX+1, indexY);
    it = blockMap.find(index);
    if(it != blockMap.end()){
        it->second->leftBlock = bb;
        bb->rightBlock = it->second;
    }
}

int MyGraphicsView::XYToGrid(int x){
    if(x < 0){
        return x / BLOCK_SIZE * BLOCK_SIZE - BLOCK_SIZE;
    }
    else{
        return x / BLOCK_SIZE * BLOCK_SIZE;
    }
}

int MyGraphicsView::XYToIndex(int x){
    return XYToGrid(x) / BLOCK_SIZE;
}

int MyGraphicsView::indexToXYCenter(int index){
    int res = index * BLOCK_SIZE + BLOCK_SIZE/2;
    return res;
}

void MyGraphicsView::blockUpdate(){
    MyHashMap::iterator it;
    for(it = blockMap.begin(); it != blockMap.end(); it++){
        it->second->updateState();
    }
    for(it = blockMap.begin(); it != blockMap.end(); it++){
        it->second->stateSwap();
    }
    scene()->update();
}

void MyGraphicsView::startTimer(int breakTime){
    blockUpdateTimer->start(breakTime);
}

void MyGraphicsView::stopTimer(){
    blockUpdateTimer->stop();
}

void MyGraphicsView::setBlockType(int type){
    blockType = type;
}

void MyGraphicsView::resetBlock(){
    MyHashMap::iterator it;
    for(it = blockMap.begin(); it != blockMap.end(); it++){
        it->second->reset();
    }
    scene()->update();
}

void MyGraphicsView::deleteBlock(IndexPair index){
    MyHashMap::iterator it = blockMap.find(index);
    if(it != blockMap.end()){
        BasicBlock* bb = it->second;
        if(bb->upBlock){
            bb->upBlock->downBlock = NULL;
        }
        if(bb->downBlock){
            bb->downBlock->upBlock = NULL;
        }
        if(bb->leftBlock){
            bb->leftBlock->rightBlock = NULL;
        }
        if(bb->rightBlock){
            bb->rightBlock->leftBlock = NULL;
        }
        blockMap.erase(it);
        scene()->removeItem(bb);
        delete bb;
    }
    scene()->update();
}

void MyGraphicsView::interactiveBlock(IndexPair index){
    MyHashMap::iterator it = blockMap.find(index);
    if(it != blockMap.end()){
        it->second->interactive();
    }
    scene()->update();
}

void MyGraphicsView::setMyScene(MyScene *s){
    myScene = s;
    myScene->lightSet = lightSet;
    myScene->selectPoint1IsValid = &selectPoint1IsValid;
    myScene->selectPoint2IsValid = &selectPoint2IsValid;
    myScene->selectPoint1 = &selectPoint1;
    myScene->selectPoint2 = &selectPoint2;
    myScene->selectMaxX = &selectMaxX;
    myScene->selectMaxY = &selectMaxY;
    myScene->selectMinX = &selectMinX;
    myScene->selectMinY = &selectMinY;
}

void MyGraphicsView::setSelectMaxMinXY(){
    selectMinX = std::min(selectPoint1.first, selectPoint2.first);
    selectMaxX = std::max(selectPoint1.first, selectPoint2.first)+1;
    selectMinY = std::min(selectPoint1.second, selectPoint2.second);
    selectMaxY = std::max(selectPoint1.second, selectPoint2.second)+1;
}

void MyGraphicsView::deleteSelectArea(){
    if(!selectPoint1IsValid || !selectPoint2IsValid){
        return;
    }
    IndexPair index;
    for(int i = selectMinX; i < selectMaxX; ++i){
        for(int j = selectMinY; j < selectMaxY; ++j){
            index.first = i;
            index.second = j;
            deleteBlock(index);
        }
    }
}

std::vector<BlockInfo> MyGraphicsView::getSelectAreaBlockInfo(){
    MyHashMap::iterator it;
    IndexPair index;
    std::vector<BlockInfo> res;
    for(int i = selectMinX; i < selectMaxX; ++i){
        for(int j = selectMinY; j < selectMaxY; ++j){
            index.first = i;
            index.second = j;
            it = blockMap.find(index);
            if(it != blockMap.end()){
                BlockInfo bi;
                bi.first.first = index.first - selectMinX;
                bi.first.second = index.second - selectMinY;
                bi.second = it->second->blockType;
                res.push_back(bi);
            }
        }
    }
    return res;
}

void MyGraphicsView::pasteSelectArea(int indexX, int indexY){
    if(!selectPoint1IsValid || !selectPoint2IsValid){
        return;
    }
    std::vector<BlockInfo> blocks = getSelectAreaBlockInfo();
    pasteAreaByVector(indexX, indexY, blocks);
}

void MyGraphicsView::pasteAreaByVector(int indexX, int indexY, std::vector<BlockInfo> blocks){
    for(int i = 0; i < blocks.size(); ++i){
        int x = blocks[i].first.first + indexX;
        int y = blocks[i].first.second + indexY;
        addBlock(indexToXYCenter(x), indexToXYCenter(y), blocks[i].second);
    }
}

void MyGraphicsView::clearSelectPoint(){
    selectPoint1IsValid = false;
    selectPoint2IsValid = false;
    myScene->update();
}
