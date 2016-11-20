#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QObject>
#include <QtWidgets>

class RenderArea : public QWidget
{
    Q_OBJECT
public:
    enum class PenStyle { SIMPLE, BRUSH };
    enum class DrawForm { DOT, LINE, RECT };

    explicit RenderArea(QWidget *parent = 0);
    ~RenderArea();

signals:
    void getMousePos(const QPoint&);

public slots:
    void setPenStyle(const PenStyle &style);
    void setDrawForm(const DrawForm &form);

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private:
    void paint();
    QPixmap image;
    DrawForm current_draw_form;
    QPen *pen;
    QPainterPath *draw_item;
    QPainter *painter;
    QPoint mouse_pos;
    volatile bool lmb_pressed;
};

#endif // RENDERAREA_H
