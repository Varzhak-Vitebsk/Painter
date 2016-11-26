#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "graphicsview.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();    

protected:
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private slots:
    void newAction();
    void openAction();
    bool saveAction();
    bool saveAsAction();
    void changeMousePos(const QPoint &point);
    void sceneChanged();

private:
    void createActions();
    void tuneWindow();
    bool changesSaved();
    void newFile();
    void openFile();
    void loadFile(const QString &file_name);
    bool saveFile(const QString &file_name);
    void setCurrentFile(const QString &file_name);

    QString cur_file;
    QLabel *mouse_pos;
    GraphicsView *view;
};

#endif // MAINWINDOW_H
