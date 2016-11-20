#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "renderarea.h"

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
      void changeMousePos(const QPoint &mouse_pos);
      void penButtonClicked();
      void drawLineButtonClicked();
      void drawRectButtonClicked();

  private:
      void createActions();
      void tuneWindow();
      bool changesSaved();
      bool saveFile(const QString &file_name);
      void setCurrentFile(const QString &file_name);

      QString cur_file;
      RenderArea *canvas;
      QLabel *mouse_pos_x_num;
      QLabel *mouse_pos_y_num;
      QPushButton *pen_button;
      QPushButton *draw_line_button;
      QPushButton *draw_rect_button;
      QPushButton *current_style_button;
      QPushButton *current_draw_form_button;
};

#endif // MAINWINDOW_H
