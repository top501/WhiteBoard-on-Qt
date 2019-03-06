#include "painterscene.h"
#include <QDebug>

PainterScene::PainterScene(QObject *parent)
    : QGraphicsScene(parent)
    , m_toolType(tt_Line)
    , m_currentShape(nullptr)
{

}

PainterScene::~PainterScene(){}

void PainterScene::setToolType(int type){
    m_toolType = type;
    if(m_currentShape != nullptr){
        if(!m_currentShape->isValid()){
            delete m_currentShape;
        }
        m_currentShape = nullptr;//下次事件发生时候就可以做相应处理
    }
}

void PainterScene::onFigureAdded(const QJsonObject &figure){
    //To do
    qDebug() << "PainterScene::onFigureAdded";
}

void PainterScene::onFigureDeleted(int id){
    //To do
    qDebug() << "PainterScene::onFigureDeleted";
}

void PainterScene::onFigureCleared(int ownerId){
    //To do
    qDebug() << "PainterScene::onFigureCleared";
}

void PainterScene::mousePressEvent(QGraphicsSceneMouseEvent *ev){
    QGraphicsScene::mousePressEvent(ev);
    if(ev->button() != Qt::LeftButton) return;//只处理鼠标左键
    if(!ev->isAccepted()){//父类还没处理
        switch(m_toolType){
        case tt_Line:
            m_currentShape = new SLine();
            m_currentShape->setStrokeColor(Qt::black);
            m_currentShape->setStrokeWidth(2);
            break;
        case tt_Circle:
            m_currentShape = new SOval();
            m_currentShape->setStrokeWidth(0);
            m_currentShape->setFillColor(Qt::green);
            break;
        case tt_Rectangle:
            m_currentShape = new SRectangle();
            m_currentShape->setStrokeWidth(4);
            break;
        case tt_Triangle:
            m_currentShape = new STriangle();
            m_currentShape->setStrokeColor(Qt::blue);
            m_currentShape->setStrokeWidth(2);
            break;
        case tt_Graffiti:
            m_currentShape = new SGraffiti();
            m_currentShape->setStrokeColor(Qt::black);
            m_currentShape->setStrokeWidth(4);
            break;//To Do
        default:
            return;

        }
        if(m_currentShape == nullptr) return;//异常，不作处理
        addItem(m_currentShape);
        m_currentShape->setStartPoint(ev->scenePos());
    }
}

void PainterScene::mouseMoveEvent(QGraphicsSceneMouseEvent *ev){
    QGraphicsScene::mouseMoveEvent(ev);
    if(!ev->isAccepted() && m_currentShape){
        m_currentShape->setEndPoint(ev->scenePos());
        update();
    }
}

void PainterScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *ev){
    QGraphicsScene::mouseReleaseEvent(ev);
    if(ev->button() != Qt::LeftButton) return;//只处理鼠标左键
    if(!ev->isAccepted() && m_currentShape){
        if(m_currentShape->isValid()){
            //To do
            QJsonObject figure;
            emit addFigureReq(figure);
        }
        else{
            removeItem(m_currentShape);
            delete m_currentShape;
        }
        m_currentShape = nullptr;
    }
}
