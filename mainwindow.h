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
    void instrumentPanelMenuAction();
    void instrumentPanelDockWidgetAction();
    void instrumentPanelDockWidgetAreaChanged(Qt::DockWidgetArea area);
    void penButtonClicked();
    void lineButtonClicked();
    void rectButtonClicked();
    void rubberButtonClicked();
    void penSpinBoxChanged(int value);
    void colorButtonClicked();


private:
    void createActions();
    void createInstrumentWidgets();
    void tuneWindow();
    bool changesSaved();
    void newFile();
    void openFile();
    void loadFile(const QString &file_name);
    bool saveFile(const QString &file_name);
    void setCurrentFile(const QString &file_name);
    void drawRubberCursor(int width);

    QString cur_file;
    const QSize panel_button_size;
    QLabel *mouse_pos;
    GraphicsView *view;
    QDockWidget *instrument_dock_widget;
    QAction *show_instrument_panel;
    QPushButton *color_button;
    QCursor rubber_cursor;

};

#endif // MAINWINDOW_H
