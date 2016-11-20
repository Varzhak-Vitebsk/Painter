#include "renderarea.h"

RenderArea::RenderArea(QWidget *parent) : QWidget(parent)
{
    //QImage *image = new QImage(QSize(height(), width()), QImage::Format_RGB32);
    QPalette palet;
    palet.setColor(QPalette::Window, Qt::white);
    setPalette(palet);
    setAutoFillBackground(true);
    setMinimumSize(640, 480);
    setMinimumSize(1024, 768);
    setMouseTracking(true);
}

RenderArea::~RenderArea()
{
    /*delete painter;
    delete image;*/
}

void RenderArea::paintEvent(QPaintEvent *event)
{
    painter = new QPainter(this);
    painter->drawLine(0, 0, 100, 100);

    //    update();
}

void RenderArea::mouseMoveEvent(QMouseEvent *event)
{
    emit getMousePos(event->pos());
}
