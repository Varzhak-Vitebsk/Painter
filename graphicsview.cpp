#include "graphicsview.h"

GraphicsView::GraphicsView(QWidget *parent):
    QGraphicsView(parent), latest_mouse_pos(QPointF(-1, -1))
{

}

GraphicsView::GraphicsView(QGraphicsScene *scene, QWidget *parent):
    QGraphicsView(scene, parent), latest_mouse_pos(QPointF(-1, -1))
{

}

void GraphicsView::mouseMoveEvent(QMouseEvent *event)
{    
    emit getMousePos(event->pos());
    if(event->buttons() == Qt::LeftButton)
    {
        if(latest_mouse_pos!=QPointF(event->pos()))
        {
            scene()->addLine(latest_mouse_pos.x(), latest_mouse_pos.y(), event->pos().x(), event->pos().y());
            latest_mouse_pos = QPointF(event->pos());
        }
    }
    event->accept();
}

void GraphicsView::mousePressEvent(QMouseEvent *event)
{
    latest_mouse_pos.setX(event->pos().x());
    latest_mouse_pos.setY(event->pos().y());
    event->accept();
}

void GraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    latest_mouse_pos.setX(-1);
    latest_mouse_pos.setY(-1);
    event->accept();
}
