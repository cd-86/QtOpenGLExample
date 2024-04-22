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
#include "BasicLightingExample/BasicLightingExample.h"
#include "MaterialsExample/MaterialsExample.h"
#include "LightingMapsExample/LightingMapsExample.h"
#include "LightCastersExample/LightCastersExample.h"
#include "MultipleLightsExample/MultipleLightsExample.h"
#include "TextRenderingExample/TextRenderingExample.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    resize(400, 800);
    m_model = new QStandardItemModel(this);
    ui->listView->setModel(m_model);
    ui->listView->setEditTriggers(QListView::NoEditTriggers);
    m_model->appendRow(new QStandardItem(QStringLiteral("创建OpenGL窗口")));
    m_model->appendRow(new QStandardItem(QStringLiteral("你好三角形")));
    m_model->appendRow(new QStandardItem(QStringLiteral("着色器")));
    m_model->appendRow(new QStandardItem(QStringLiteral("纹理")));
    m_model->appendRow(new QStandardItem(QStringLiteral("变换")));
    m_model->appendRow(new QStandardItem(QStringLiteral("坐标系统")));
    m_model->appendRow(new QStandardItem(QStringLiteral("摄像机")));
    m_model->appendRow(new QStandardItem(QStringLiteral("颜色")));
    m_model->appendRow(new QStandardItem(QStringLiteral("基础光照")));
    m_model->appendRow(new QStandardItem(QStringLiteral("材质")));
    m_model->appendRow(new QStandardItem(QStringLiteral("光照贴图")));
    m_model->appendRow(new QStandardItem(QStringLiteral("投光物")));
    m_model->appendRow(new QStandardItem(QStringLiteral("多光源")));
    m_model->appendRow(new QStandardItem(QStringLiteral("文本渲染")));
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
    case 8:
        newWindow = new BasicLightingExample;
        break;
    case 9:
        newWindow = new MaterialsExample;
        break;
    case 10:
        newWindow = new LightingMapsExample;
        break;
    case 11:
        newWindow = new LightCastersExample;
        break;
    case 12:
        newWindow = new MultipleLightsExample;
        break;
    case 13:
        newWindow = new TextRenderingExample;
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

