#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QObject>
#include <QtWidgets>

class GraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    explicit GraphicsView(QWidget *parent = Q_NULLPTR);
    explicit GraphicsView(QGraphicsScene *scene, QWidget *parent = Q_NULLPTR);

signals:
    void getMousePos(const QPoint&);

public slots:    

protected:    
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private:
    QPointF latest_mouse_pos;

};

#endif // GRAPHICSVIEW_H
