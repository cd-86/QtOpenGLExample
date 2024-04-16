#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_listView_doubleClicked(const QModelIndex &index);

    // QWidget interface
protected:
    virtual void closeEvent(QCloseEvent *event) override;

private:
    Ui::MainWindow *ui;
    QStandardItemModel *m_model;
    QWidget *w {nullptr};
};

#endif // MAINWINDOW_H
