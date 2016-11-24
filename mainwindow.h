#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "graphicsview.h"
//#include <QtWidgets>
//#include <QGraphicsScene>


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    void loadFile(const QString &file_name);

protected:
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private slots:
    void open();
    bool save();
    bool saveAs();
    void changeMousePos(const QPoint &point);

private:
    void createActions();
    void tuneWindow();
    bool changesSaved();
    bool saveFile(const QString &file_name);
    void setCurrentFile(const QString &file_name);

    QString cur_file;
    QLabel *mouse_pos;
    GraphicsView *view;
    QGraphicsScene *scene;
};

#endif // MAINWINDOW_H
