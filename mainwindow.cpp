#include "mainwindow.h"

MainWindow::MainWindow()
{
    tuneWindow();
    createActions();
    setCurrentFile(QString());
    penButtonClicked();
}

void MainWindow::loadFile(const QString &file_name)
{
    QFile file(file_name);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, tr("Application"),
                           tr("Cannot read file %1:\n%2.")
                           .arg(QDir::toNativeSeparators(file_name), file.errorString()));
        return;
    }

    setCurrentFile(file_name);
    file.close();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (changesSaved())
    {
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::open()
{
    if (changesSaved())
    {
        QString fileName = QFileDialog::getOpenFileName(this);
        if (!fileName.isEmpty()) loadFile(fileName);
    }
}

bool MainWindow::save()
{
    if (cur_file.isEmpty())
    {
        return saveAs();
    } else {
        return saveFile(cur_file);
    }
}

bool MainWindow::saveAs()
{
    QFileDialog dialog(this);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    if (dialog.exec() != QDialog::Accepted) return false;
    return saveFile(dialog.selectedFiles().first());
}

void MainWindow::changeMousePos(const QPoint &mouse_pos)
{
    mouse_pos_x_num->setText(QString().setNum(mouse_pos.x()));
    mouse_pos_y_num->setText(QString().setNum(mouse_pos.y()));
}

void MainWindow::penButtonClicked()
{
    current_style_button->setDown(false);
    current_draw_form_button->setDown(false);
    pen_button->setDown(true);
    current_style_button = pen_button;
    canvas->setPenStyle(RenderArea::PenStyle::SIMPLE);
    canvas->setDrawForm(RenderArea::DrawForm::DOT);
}

void MainWindow::drawLineButtonClicked()
{
    current_draw_form_button->setDown(false);
    draw_line_button->setDown(true);
    current_style_button = draw_line_button;
    canvas->setDrawForm(RenderArea::DrawForm::LINE);
}

void MainWindow::drawRectButtonClicked()
{
    current_draw_form_button->setDown(false);
    draw_rect_button->setDown(true);
    current_style_button = draw_rect_button;
    canvas->setDrawForm(RenderArea::DrawForm::RECT);
}

void MainWindow::createActions()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));

    QAction *openAct = new QAction(tr("&Open"), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, &QAction::triggered, this, &MainWindow::open);
    fileMenu->addAction(openAct);

    QAction *saveAct = new QAction(tr("&Save"), this);
    openAct->setShortcuts(QKeySequence::Save);
    openAct->setStatusTip(tr("Save current file"));
    connect(saveAct, &QAction::triggered, this, &MainWindow::save);
    fileMenu->addAction(saveAct);

    QAction *saveAsAct = new QAction(tr("&Save as..."), this);
    openAct->setShortcuts(QKeySequence::SaveAs);
    openAct->setStatusTip(tr("Save current file as..."));
    connect(saveAsAct, &QAction::triggered, this, &MainWindow::saveAs);
    fileMenu->addAction(saveAsAct);
}

void MainWindow::tuneWindow()
{
    QFrame *frame = new QFrame;
    QVBoxLayout *frameLayout = new QVBoxLayout(frame);

    //----frame layout objects
    QHBoxLayout *centralLayout = new QHBoxLayout();
    QHBoxLayout *bottomLayout = new QHBoxLayout();

    //----central layout objects
    QVBoxLayout *instrumentLayout = new QVBoxLayout();
    QScrollArea *canvas_area = new QScrollArea();
    canvas = new RenderArea();

    //----bottom layout objects
    QLabel *mouse_pos_x_str = new QLabel("X:");
    mouse_pos_x_str->setFixedSize(20, 20);
    mouse_pos_x_num = new QLabel();
    mouse_pos_x_num->setFixedHeight(20);
    QLabel *mouse_pos_y_str = new QLabel("Y:");
    mouse_pos_y_str->setFixedSize(20, 20);
    mouse_pos_y_num = new QLabel();
    mouse_pos_y_num->setFixedHeight(20);
    connect(canvas, SIGNAL(getMousePos(QPoint)), this, SLOT(changeMousePos(QPoint)));

    //----instrument layout objects
    QSize instrument_buttons_size(50, 50);
    pen_button = new QPushButton("Pen");
    pen_button->setFixedSize(instrument_buttons_size);
    current_style_button = pen_button;
    current_draw_form_button = pen_button;
    connect(pen_button, SIGNAL(clicked(bool)), this, SLOT(penButtonClicked()));
    draw_line_button = new QPushButton("Line");
    draw_line_button->setFixedSize(instrument_buttons_size);
    connect(draw_line_button, SIGNAL(clicked(bool)), this, SLOT(drawLineButtonClicked()));
    draw_rect_button = new QPushButton("Rect");
    draw_rect_button->setFixedSize(instrument_buttons_size);
    connect(draw_rect_button, SIGNAL(clicked(bool)), this, SLOT(drawRectButtonClicked()));

    //----placing and setting layots
    frameLayout->addLayout(centralLayout);
    frameLayout->addLayout(bottomLayout);
    centralLayout->addLayout(instrumentLayout);
    centralLayout->addWidget(canvas_area);
    centralLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    centralLayout->setSizeConstraint(QLayout::SetMinimumSize);
    bottomLayout->addWidget(mouse_pos_x_str);
    bottomLayout->addWidget(mouse_pos_x_num);
    bottomLayout->addWidget(mouse_pos_y_str);
    bottomLayout->addWidget(mouse_pos_y_num);
    bottomLayout->setAlignment(Qt::AlignLeft);
    canvas_area->setWidget(canvas);
    instrumentLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    instrumentLayout->setSizeConstraint(QLayout::SetNoConstraint);
    instrumentLayout->addWidget(pen_button);
    instrumentLayout->addWidget(draw_line_button);
    instrumentLayout->addWidget(draw_rect_button);

    //----main window settings
    setCentralWidget(frame);
    setMinimumSize(400, 500);
}

bool MainWindow::changesSaved()
{
    return true;
}

bool MainWindow::saveFile(const QString &file_name)
{
    QFile file(file_name);
    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this, tr("Application"),
                            tr("Cannot write file %1:\n%2.")
                            .arg(QDir::toNativeSeparators(file_name),
                            file.errorString()));
        return false;
    }

    file.close();
    return true;
}

void MainWindow::setCurrentFile(const QString &file_name)
{
    cur_file = file_name;
    setWindowModified(false);
    QString shownName = cur_file;
    if (cur_file.isEmpty()) shownName = "untitled.txt";
    setWindowFilePath(shownName);
}
