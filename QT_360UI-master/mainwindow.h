#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPoint>
#include <QMenu>
#include <QAction>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    /* AboutpushButton按钮下的菜单 */
    QAction *AboutAct;
    QAction *SettingAct;


private slots:

    void on_closeToolButton_clicked();

    void on_MinitoolButton_clicked();

    void on_ChecktoolButton_clicked();

    void on_KilltoolButton_clicked();

    void on_FixtoolButton_clicked();

    void on_CleantoolButton_clicked();

    void on_SpeedtoolButton_clicked();

    void on_ComputertoolButton_clicked();

    void on_SofttoolButton_clicked();

    void AboutMessageBox();
private:
    Ui::MainWindow *ui;

    /* 创建子窗口 */
    void setStackCurrentPage(quint8 index);

    /* 鼠标拖动窗口 */
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);

    /* 记录窗口当前的位置 */
    QPoint StartPos;
};

#endif // MAINWINDOW_H
