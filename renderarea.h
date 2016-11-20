#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QObject>
#include <QtWidgets>

class RenderArea : public QWidget
{
    Q_OBJECT
public:
    explicit RenderArea(QWidget *parent = 0);
    ~RenderArea();

signals:
    void getMousePos(const QPoint&);

public slots:

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private:
    QImage *image;
    QPainter *painter;
};

#endif // RENDERAREA_H
