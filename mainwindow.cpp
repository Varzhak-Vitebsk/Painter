#include "mainwindow.h"

MainWindow::MainWindow()
{    
    tuneWindow();
    createActions();
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

void MainWindow::createActions()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));

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
}

void MainWindow::tuneWindow()
{    
    setMinimumSize(700, 600);

    QFrame *frame = new QFrame;
    view = new GraphicsView(frame);
    view->setMouseTracking(true);
    newFile();

    mouse_pos = new QLabel();
    statusBar()->addPermanentWidget(mouse_pos);
    statusBar()->setLayoutDirection(Qt::RightToLeft);
    view->show();

    //----main window settings
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
