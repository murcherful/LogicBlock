#include "mygraphicsview.h"

MyGraphicsView::MyGraphicsView(QWidget* parent):QGraphicsView(parent){
    zoomScale = 1;
    centerX = 0;
    centerY = 0;
    isEdit = false;
    isLeftMouseMoving = false;
    this->setCursor(Qt::SizeAllCursor);
    blockUpdateTimer = new QTimer(this);
    connect(blockUpdateTimer, SIGNAL(timeout()), this, SLOT(blockUpdate()));
    blockType = BasicBlock::POWER;
    lightSet = new MyHashSet;
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
    if(event->button() == Qt::LeftButton){
        if(isEdit){
            QPointF scenePoint = this->mapToScene(event->pos());
            addBlock(scenePoint.x(), scenePoint.y());
        }
        else{
            startPoint = event->pos();
            isLeftMouseMoving = true;
        }
    }
    else if(event->button() == Qt::RightButton){
        QPointF scenePoint = this->mapToScene(event->pos());
        int indexX = transToIndex(scenePoint.x());
        int indexY = transToIndex(scenePoint.y());
        IndexPair index(indexX, indexY);
        if(isEdit){
            deleteBlock(index);
        }
        else{
            interactiveBlock(index);
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
}

void MyGraphicsView::mouseReleaseEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        if(isEdit){

        }
        else{
            QPointF disPoint = event->pos() - startPoint;
            disPoint /= zoomScale;
            scene()->setSceneRect(centerX-disPoint.x(), centerY-disPoint.y(), SCENE_WIDTH, SCENE_HEIGHT);
            scene()->update();
            centerX = centerX-disPoint.x();
            centerY = centerY-disPoint.y();
            isLeftMouseMoving = false;
        }
    }
}

void MyGraphicsView::setIsEdit(bool s){
    isEdit = s;
    if(!isEdit){
        this->setCursor(Qt::SizeAllCursor);
    }
    else{
        this->setCursor(Qt::ArrowCursor);
    }
}

void MyGraphicsView::addBlock(int x, int y){
    x = transToGrid(x);
    y = transToGrid(y);
    int indexX = x / BLOCK_SIZE;
    int indexY = y / BLOCK_SIZE;
    IndexPair index(indexX, indexY);
    MyHashMap::iterator it = blockMap.find(index);
    if(it != blockMap.end()){
       return;
    }
    BasicBlock* bb = getBlock(x, y);
    blockMap.insert(std::pair<IndexPair, BasicBlock*>(index, bb));
    combineBlock(bb);
    this->scene()->addItem(bb);
    this->scene()->update();
}

BasicBlock* MyGraphicsView::getBlock(int x, int y){
    if(blockType == BasicBlock::POWER){
        return new PowerBlock(x, y);
    }
    else if(blockType == BasicBlock::STRAIGHT_LINE_H){
        return new StraightLineBlock(x, y, StraightLineBlock::HORIZONTAL);
    }
    else if(blockType == BasicBlock::STRAIGHT_LINE_V){
        return new StraightLineBlock(x, y, StraightLineBlock::VERTICAL);
    }
    else if(blockType == BasicBlock::LEFT_UP_LINE){
        return new CornerBlock(x, y, CornerBlock::LEFT_UP);
    }
    else if(blockType == BasicBlock::LEFT_DOWN_LINE){
        return new CornerBlock(x, y, CornerBlock::LEFT_DOWN);
    }
    else if(blockType == BasicBlock::RIGHT_UP_LINE){
        return new CornerBlock(x, y, CornerBlock::RIGHT_UP);
    }
    else if(blockType == BasicBlock::RIGHT_DOWN_LINE){
        return new CornerBlock(x, y, CornerBlock::RIGHT_DOWN);
    }
    else if(blockType == BasicBlock::LIGHT){
        return new LightBlock(x, y, lightSet);
    }
    else if(blockType == BasicBlock::CROSS_CONNECT){
        return new CrossConnectBlock(x, y);
    }
    else if(blockType == BasicBlock::CROSS_NOT_CONNECT){
        return new CrossNotConnectBlock(x, y);
    }
    else if(blockType == BasicBlock::SWITCH_BLOCK){
        return new SwitchBlock(x, y);
    }
    else if(blockType == BasicBlock::AND_GATE_BLOCK){
        return new AndGateBlock(x, y);
    }
    else if(blockType == BasicBlock::OR_GATE_BLOCK){
        return new OrGateBlock(x, y);
    }
    else if(blockType == BasicBlock::NOT_GATE_BLOCK){
        return new NotGateBlock(x, y);
    }
    else if(blockType == BasicBlock::XOR_GATE_BLOCK){
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

int MyGraphicsView::transToGrid(int x){
    if(x < 0){
        return x / BLOCK_SIZE * BLOCK_SIZE - BLOCK_SIZE;
    }
    else{
        return x / BLOCK_SIZE * BLOCK_SIZE;
    }
}

int MyGraphicsView::transToIndex(int x){
    return transToGrid(x) / BLOCK_SIZE;
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
