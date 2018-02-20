#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Create 64 x 32 display
    scene = new QGraphicsScene(this);
    pixmap = new QPixmap(64, 32);
    pixmapItem = scene->addPixmap(*pixmap);

    // Set grahicView's scene as the one that we created
    ui->graphicsView->setScene(scene);

    // Scale to see the scene bigger
    ui->graphicsView->scale(SCALE_FACTOR, SCALE_FACTOR);
    ui->graphicsView->setChip8(&chip8);

    // Scene painter
    painter = new QPainter();

    // FPS counter times
    fpsTimer.start();
    fpsCounter = 0;

    // When chip8.drawScreen signal is emitted, activate mainwindow.drawGraphic function
    // This is for drawing the new scene.
    connect(&chip8, &Chip8::drawScreen, this, &MainWindow::drawGraphic);

    // When scene.changed signal is emitted, activate graphicsView.updateScene function
    // This is updating the graphics in the coordinate system.
    connect(scene, SIGNAL(changed(QList<QRectF>)), ui->graphicsView, SLOT(updateScene(QList<QRectF>)));

    // Initialize chip8 to run it.
    chip8.initialize();
}

MainWindow::~MainWindow()
{
    delete ui;
}


/**
 * Draw the latest situation. This function will be
 * runned by a signal.
 */
void MainWindow::drawGraphic()
{
    painter->begin(pixmap);
    for (int i = 0; i < (64 * 32); i++)
    {
        if( chip8.syncgraphic )
        {
            if (chip8.gfxSync[i])
                painter->setPen(Qt::white);
            else
                painter->setPen(Qt::black);
        }
        else
        {
            if (chip8.gfx[i])
                painter->setPen(Qt::white);
            else
                painter->setPen(Qt::black);
        }
        painter->drawPoint(i % 64, i / 64);
    }
    painter->end();
    pixmapItem->setPixmap(*pixmap);
    //qDebug() << fpsTimer.elapsed() << endl;
    fpsCounter ++;
    if( fpsTimer.elapsed() > 1000.0)
    {
        qDebug() << fpsCounter << endl;
        fpsTimer.restart();
        fpsCounter = 0;
    }
}

/**
 * Starts emulation when the function is called.
 */
void MainWindow::startEmulation()
{
    chip8.startEmulation();
}

/**
 * Stops emulation when the function is called.
 */
void MainWindow::stopEmulation()
{
    chip8.stopEmulation();
}

/**
 * Run the following function when start button is clicked.
 */
void MainWindow::on_actionStart_Emulation_triggered()
{
    startEmulation();
}

/**
 * Run the following function when stop button is clicked.
 */
void MainWindow::on_actionStop_Emulation_triggered()
{
    stopEmulation();
}

void MainWindow::on_actionToggle_Sync_triggered()
{
    if( chip8.syncgraphic )
    {
        ui->actionToggle_Sync->setText("Enable Graphic Sync");
        chip8.syncgraphic = false;
        return;
    }

    ui->actionToggle_Sync->setText("Disable Graphic Sync");
    chip8.syncgraphic = true;
}
