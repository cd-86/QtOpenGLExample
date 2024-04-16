#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "CreateOpenGLWindowExample/CreateOpenGLWindowExample.h"
#include "HelloTriangleExample/HelloTriangleExample.h"
#include "ShadersExample/ShadersExample.h"
#include "TexturesExample/TexturesExample.h"
#include "TransformationsExample/TransformationsExample.h"
#include "CoordinateSystemsExample/CoordinateSystemsExample.h"
#include "CameraExample/CameraExample.h"
#include "ColorsExample/ColorsExample.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    resize(400, 800);
    m_model = new QStandardItemModel(this);
    ui->listView->setModel(m_model);
    ui->listView->setEditTriggers(QListView::NoEditTriggers);
    m_model->appendRow(new QStandardItem("Create OpenGLWidget"));
    m_model->appendRow(new QStandardItem("Hello Triangle"));
    m_model->appendRow(new QStandardItem("Shaders"));
    m_model->appendRow(new QStandardItem("Textures"));
    m_model->appendRow(new QStandardItem("Transformations"));
    m_model->appendRow(new QStandardItem("Coordinate Systems"));
    m_model->appendRow(new QStandardItem("Camera"));
    m_model->appendRow(new QStandardItem("Colors"));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete w;
}

void MainWindow::on_listView_doubleClicked(const QModelIndex &index)
{
    QWidget *newWindow {nullptr};
    switch(index.row()) {
    case 0:
        newWindow = new CreateOpenGLWindowExample;
        break;
    case 1:
        newWindow = new HelloTriangleExample;
        break;
    case 2:
        newWindow = new ShadersExample;
        break;
    case 3:
        newWindow = new TexturesExample;
        break;
    case 4:
        newWindow = new TransformationsExample;
        break;
    case 5:
        newWindow = new CoordinateSystemsExample;
        break;
    case 6:
        newWindow = new CameraExample;
        break;
    case 7:
        newWindow = new ColorsExample;
        break;
    }
    if (newWindow){
        if (w) {
            newWindow->resize(w->size());
            newWindow->move(w->pos());
            w->close();
            delete w;
        }
        w = newWindow;
        w->setWindowTitle(index.data().toString());
        w->show();
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(w) w->close();
}

