#include "mainwindow.h"

MainWindow::MainWindow():
    panel_button_size(QSize(50, 50))
{    
    tuneWindow();
    createActions();
    createInstrumentWidgets();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (changesSaved())
    {
        event->accept();
    } else {
        QMessageBox msgBox;
        msgBox.setText("The document has been modified.");
        msgBox.setInformativeText("Do you want to save your changes?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();
        switch (ret) {
            case QMessageBox::Save:
                saveAction();
                event->accept();
                break;
            case QMessageBox::Discard:
                event->accept();
                break;
            case QMessageBox::Cancel:
                event->ignore();
                break;
            default:
                event->ignore();
                break;
        }
    }
}

void MainWindow::newAction()
{
    if (changesSaved())
    {
        newFile();
    } else
    {
        QMessageBox msgBox;
        msgBox.setText("The document has been modified.");
        msgBox.setInformativeText("Do you want to save your changes?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();
        switch (ret) {
            case QMessageBox::Save:
                saveAction();
                newFile();
                break;
            case QMessageBox::Discard:
                newFile();
                break;
            case QMessageBox::Cancel:
                break;
            default:
                break;
        }
    }

}

void MainWindow::openAction()
{
    if (changesSaved())
    {
       openFile();
    } else
    {
        QMessageBox msgBox;
        msgBox.setText("The document has been modified.");
        msgBox.setInformativeText("Do you want to save your changes?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();
        switch (ret) {
            case QMessageBox::Save:
                saveAction();
                openFile();
                break;
            case QMessageBox::Discard:
                openFile();
                break;
            case QMessageBox::Cancel:
                break;
            default:
                break;
        }
    }
}

bool MainWindow::saveAction()
{
    if(cur_file.isEmpty())
    {
        return saveAsAction();
    } else {
        return saveFile(cur_file);
    }
}

bool MainWindow::saveAsAction()
{
    QFileDialog dialog(this);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setNameFilter(tr("*.png"));
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    if (dialog.exec() != QDialog::Accepted) return false;
    return saveFile(dialog.selectedFiles().first());
}

void MainWindow::changeMousePos(const QPoint &point)
{
    mouse_pos->setText("X: " + QString().setNum(point.x())
                       + " Y: " +  QString().setNum(point.y()));
}

void MainWindow::sceneChanged()
{
    //qDebug() << "Scene changed";
    setWindowModified(true);
}

void MainWindow::instrumentPanelDockWidgetAction()
{
    instrument_dock_widget->isVisible() ? show_instrument_panel->setChecked(true) : show_instrument_panel->setChecked(false);
}

void MainWindow::penButtonClicked()
{
    view->setDrawForm(DrawForm::DOT);
}

void MainWindow::lineButtonClicked()
{
    view->setDrawForm(DrawForm::LINE);
}

void MainWindow::rectButtonClicked()
{
    view->setDrawForm(DrawForm::RECT);
}

void MainWindow::instrumentPanelDockWidgetAreaChanged(Qt::DockWidgetArea area)
{
    QBoxLayout *layout = instrument_dock_widget->findChild < QBoxLayout* >();
    if(layout)
    {
        if((area & Qt::LeftDockWidgetArea) || (area & Qt::RightDockWidgetArea))
        {
            layout->setDirection(QBoxLayout::TopToBottom);
            layout->setAlignment(Qt::AlignCenter | Qt::AlignTop);
        }
        if((area & Qt::TopDockWidgetArea) || (area & Qt::BottomDockWidgetArea))
        {
            layout->setDirection(QBoxLayout::LeftToRight);
            layout->setAlignment(Qt::AlignCenter | Qt::AlignLeft);
        }
    }
}

void MainWindow::instrumentPanelMenuAction()
{
    show_instrument_panel->isChecked() ? instrument_dock_widget->show() : instrument_dock_widget->hide();
}

void MainWindow::createActions()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    // File menu actions
    QAction *newAct = new QAction(tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, &QAction::triggered, this, &MainWindow::newAction);
    fileMenu->addAction(newAct);

    QAction *openAct = new QAction(tr("&Open"), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, &QAction::triggered, this, &MainWindow::openAction);
    fileMenu->addAction(openAct);

    QAction *saveAct = new QAction(tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save current file"));
    connect(saveAct, &QAction::triggered, this, &MainWindow::saveAction);
    fileMenu->addAction(saveAct);

    QAction *saveAsAct = new QAction(tr("Save as..."), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Save current file as..."));
    connect(saveAsAct, &QAction::triggered, this, &MainWindow::saveAsAction);
    fileMenu->addAction(saveAsAct);

    QAction *exitAct = new QAction(tr("&Exit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit"));
    connect(exitAct, SIGNAL(triggered(bool)), qApp, SLOT(closeAllWindows()));
    fileMenu->addAction(exitAct);

    QMenu *panelsMenu = menuBar()->addMenu(tr("&Panels"));
    // Instrument menu actions
    show_instrument_panel = new QAction(tr("&Instrumet panel"), this);
    show_instrument_panel->setCheckable(true);
    show_instrument_panel->setChecked(true);
    show_instrument_panel->setStatusTip(tr("Ahow instrument panel"));
    connect(show_instrument_panel, &QAction::changed, this, &MainWindow::instrumentPanelMenuAction);
    panelsMenu->addAction(show_instrument_panel);
}

void MainWindow::createInstrumentWidgets()
{
    // DockWidget with instrument buttons
    instrument_dock_widget = new QDockWidget("Instruments");    
    connect(instrument_dock_widget, SIGNAL(visibilityChanged(bool)),
            this, SLOT(instrumentPanelDockWidgetAction()));
    connect(instrument_dock_widget, SIGNAL(dockLocationChanged(Qt::DockWidgetArea)),
            this, SLOT(instrumentPanelDockWidgetAreaChanged(Qt::DockWidgetArea)));
    QWidget *instrument_panel = new QWidget();
    instrument_panel->setMinimumWidth(150);
    QButtonGroup *instrument_buttons_group = new QButtonGroup(this);

    QBoxLayout *instrument_widget_layout = new QBoxLayout(QBoxLayout::TopToBottom, instrument_panel);
    instrument_widget_layout->setAlignment(Qt::AlignCenter | Qt::AlignTop);
    instrument_panel->setLayout(instrument_widget_layout);

    QPushButton *pen_button = new QPushButton("Pen");
    pen_button->setMinimumSize(panel_button_size);
    pen_button->setMaximumSize(panel_button_size);
    connect(pen_button, SIGNAL(clicked(bool)), this, SLOT(penButtonClicked()));
    instrument_widget_layout->addWidget(pen_button);
    pen_button->setCheckable(true);
    pen_button->setChecked(true);
    instrument_buttons_group->addButton(pen_button);

    QPushButton *line_button = new QPushButton("Line");
    line_button->setMinimumSize(panel_button_size);
    line_button->setMaximumSize(panel_button_size);
    connect(line_button, SIGNAL(clicked(bool)), this, SLOT(lineButtonClicked()));
    instrument_widget_layout->addWidget(line_button);
    line_button->setCheckable(true);
    instrument_buttons_group->addButton(line_button);

    QPushButton *rect_button = new QPushButton("Rect");
    rect_button->setMinimumSize(panel_button_size);
    rect_button->setMaximumSize(panel_button_size);
    connect(rect_button, SIGNAL(clicked(bool)), this, SLOT(rectButtonClicked()));
    instrument_widget_layout->addWidget(rect_button);
    rect_button->setCheckable(true);
    instrument_buttons_group->addButton(rect_button);

    instrument_dock_widget->setWidget(instrument_panel);
    addDockWidget(Qt::LeftDockWidgetArea, instrument_dock_widget);
}

void MainWindow::tuneWindow()
{    
    qApp->setStyle("Windows");    
    setMinimumSize(800, 600);

    QFrame *frame = new QFrame;
    view = new GraphicsView(frame);
    view->setMouseTracking(true);    

    newFile();

    mouse_pos = new QLabel();
    statusBar()->addPermanentWidget(mouse_pos);
    statusBar()->setLayoutDirection(Qt::RightToLeft);
    view->show();

    setCentralWidget(frame);
    connect(view, SIGNAL(getMousePos(QPoint)), this, SLOT(changeMousePos(QPoint)));    
}

bool MainWindow::changesSaved()
{
    return !isWindowModified();
}

void MainWindow::newFile()
{
    if(view->scene()) delete(view->scene());
    QGraphicsScene *scene = new QGraphicsScene();
    scene->setSceneRect(0, 0, 640, 480);
    view->setScene(scene);
    view->update();
    connect(scene, SIGNAL(changed(QList<QRectF>)), this, SLOT(sceneChanged()));
    setCurrentFile(QString());
}

void MainWindow::openFile()
{
    QFileDialog dialog(this);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setNameFilter(tr("Images (*.png)"));
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    QString file_name = dialog.getOpenFileName(this);
    if (!file_name.isEmpty())
    {
        newFile();
        loadFile(file_name);
        setCurrentFile(file_name);
    }
}

void MainWindow::loadFile(const QString &file_name)
{
    QImage tmp_image(file_name, "PNG");
    view->scene()->addPixmap(QPixmap::fromImage(tmp_image));
}

bool MainWindow::saveFile(const QString &file_name)
{
    QImage tmp_image(QSize(640, 480), QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&tmp_image);
    view->render(&painter);
    if(tmp_image.save(file_name, "PNG", 100))
    {
        setWindowModified(false);
    }
    painter.end();
    return true;
}

void MainWindow::setCurrentFile(const QString &file_name)
{
    cur_file = file_name;
    setWindowModified(false);
    QString shownName = cur_file;
    if (cur_file.isEmpty()) shownName = "untitled.png";
    setWindowFilePath(shownName);
    //qDebug() << "Set cur file";
}
