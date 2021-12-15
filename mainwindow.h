#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QColor>
#include <QMouseEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_lineEdit_hex_returnPressed();
    void on_lineEdit_rgb_returnPressed();

private:
    QString RgbToHex(QString rgb);
    QString HexToRgb(QString hex);
    void setLabelColor(QColor color);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
