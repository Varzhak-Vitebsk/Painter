#include "graphicsview.h"

GraphicsView::GraphicsView(QWidget *parent):
    QGraphicsView(parent), latest_mouse_pos(QPointF(-1, -1)), initial_mouse_pos(QPointF(-1, -1))
  , current_draw_form(DrawForm::DOT), current_item(Q_NULLPTR), pen(new QPen())
{

}

void GraphicsView::setDrawForm(DrawForm draw_form)
{
    current_draw_form = draw_form;
}

void GraphicsView::setPenSize(int size)
{
    pen->setWidth(size);
}

void GraphicsView::setPenColor(QColor color)
{
    pen->setColor(color);
}

void GraphicsView::mouseMoveEvent(QMouseEvent *event)
{    
    emit getMousePos(event->pos());
    if(event->buttons() == Qt::LeftButton)
    {

        switch(current_draw_form)
        {
            case DrawForm::DOT:
                if(latest_mouse_pos!=QPointF(event->pos()))
                {
                    scene()->addLine(latest_mouse_pos.x(), latest_mouse_pos.y()
                                     , static_cast<qreal>(event->pos().x())
                                     , static_cast<qreal>(event->pos().y())
                                     , *pen);
                    latest_mouse_pos = QPointF(event->pos());
                }
                break;
            case DrawForm::LINE:
                if(!current_item)
                {
                    initial_mouse_pos.setX(event->pos().x());
                    initial_mouse_pos.setY(event->pos().y());
                    current_item = scene()->addLine(event->pos().x(), event->pos().y()
                                                    , event->pos().x() + 1, event->pos().y() + 1
                                                    , *pen);
                    current_item->setTransformOriginPoint(initial_mouse_pos);
                } else
                {                    
                    qgraphicsitem_cast < QGraphicsLineItem * > (current_item)->setLine(initial_mouse_pos.x()
                                                                                       , initial_mouse_pos.y()
                                                                                       , event->pos().x()
                                                                                       , event->pos().y());
                }
                break;
            case DrawForm::RECT:
                if(!current_item)
                {
                    initial_mouse_pos.setX(event->pos().x());
                    initial_mouse_pos.setY(event->pos().y());
                    current_item = scene()->addRect(event->pos().x(), event->pos().y()
                                                    , 1, 1, *pen);
                    current_item->setTransformOriginPoint(initial_mouse_pos);
                } else
                {
                    QPointF top_left, bottom_right;
                    if((initial_mouse_pos.x() <= event->pos().x()) && (initial_mouse_pos.y() <= event->pos().y()))
                    {
                        top_left = initial_mouse_pos;
                        bottom_right = QPointF(event->pos());
                    }
                    if((initial_mouse_pos.x() > event->pos().x()) && (initial_mouse_pos.y() < event->pos().y()))
                    {
                        top_left = QPointF(event->pos().x(), initial_mouse_pos.y());
                        bottom_right = QPointF(initial_mouse_pos.x(), event->pos().y());
                    }
                    if((initial_mouse_pos.x() >= event->pos().x()) && (initial_mouse_pos.y() >= event->pos().y()))
                    {
                        top_left = QPointF(event->pos());
                        bottom_right = initial_mouse_pos;
                    }
                    if((initial_mouse_pos.x() < event->pos().x()) && (initial_mouse_pos.y() > event->pos().y()))
                    {
                        top_left = QPointF(initial_mouse_pos.x(), event->pos().y());
                        bottom_right = QPointF(event->pos().x(), initial_mouse_pos.y());
                    }
                    qgraphicsitem_cast < QGraphicsRectItem * > (current_item)->setRect(QRectF(top_left, bottom_right));
                }
                break;
            case DrawForm::RUBBER:
                scene()->addEllipse(event->pos().x() - pen->width() / 2
                                    , event->pos().y() - pen->width() / 2
                                    , pen->width(), pen->width()
                                    , QPen(Qt::white), QBrush(Qt::white));
                break;
            default:
            // some code for rotation and scaling, scaling works fine, rotation needs tuning >_<
            /*double scale = qSqrt(qPow((initial_mouse_pos.x() - event->pos().x()), 2) +  qPow((initial_mouse_pos.y() - event->pos().y()), 2))
                    / qSqrt(qPow(current_item->boundingRect().height(), 2) +  qPow(current_item->boundingRect().width(), 2));

            //qDebug() << scale;
            current_item->setScale(scale);
            double x1 = event->pos().x() - initial_mouse_pos.x();
            double y1 = event->pos().y() - initial_mouse_pos.y();
            double x2 = current_item->boundingRect().bottomRight().x() - initial_mouse_pos.x();
            double y2 = current_item->boundingRect().bottomRight().y() - initial_mouse_pos.y();
            double cos_angle = (x1 * x2 + y1 * y2) / (qSqrt(qPow(x1, 2) + qPow(y1, 2)) * qSqrt(qPow(x2, 2) + qPow(y2, 2)));
            double rotation_angle = qRadiansToDegrees(qAcos(cos_angle));
            current_item->setRotation(rotation_angle);*/
                break;

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
    current_item = Q_NULLPTR;
    event->accept();
}
