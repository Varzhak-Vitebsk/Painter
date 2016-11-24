#include "renderarea.h"

RenderArea::RenderArea(QWidget *parent) : QWidget(parent), image(QPixmap()), current_draw_form(DrawForm::DOT),
    pen(new QPen()), mouse_pos(QPoint(0, 0)), lmb_pressed(false)
{
    QPalette palet;
    palet.setColor(QPalette::Window, Qt::white);
    setPalette(palet);
    setAutoFillBackground(true);
    setMinimumSize(640, 480);
    setMaximumSize(1024, 768);
    setMouseTracking(true);
    painter = new QPainter(this);
    image = grab();
}

RenderArea::~RenderArea()
{
    /*delete painter;
    delete image;*/
}

void RenderArea::setPenStyle(const RenderArea::PenStyle &style)
{
    switch(style)
    {
    case PenStyle::SIMPLE:
        pen->setWidth(0);
        break;
    case PenStyle::BRUSH:
        pen->setWidth(20);
        break;
    default:
        break;
    }
}

void RenderArea::setDrawForm(const RenderArea::DrawForm &form)
{
    current_draw_form = form;
}

void RenderArea::paintEvent(QPaintEvent *event)
{
    painter->begin(this);
    painter->drawPixmap(0, 0, image);
    painter->end();
}

void RenderArea::mouseMoveEvent(QMouseEvent *event)
{
    mouse_pos = event->pos();
    emit getMousePos(event->pos());
    if(event->buttons() == Qt::LeftButton) paint();
}

void RenderArea::mousePressEvent(QMouseEvent *event)
{

}

void RenderArea::mouseReleaseEvent(QMouseEvent *event)
{

}

void RenderArea::paint()
{
    painter->begin(&image);
    painter->setPen(*pen);
    switch(current_draw_form)
    {
    case DrawForm::DOT:
        painter->drawPoint(mouse_pos);
        break;
    case DrawForm::LINE:
        break;
    case DrawForm::RECT:
        break;
    }
    painter->end();
    this->update();
}
